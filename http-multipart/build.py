#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Please run the script at the root of source directory.
# Run python(3) build.py --help for details.
# Set environment variable:
#  - `GENERATOR` for specifying generator; unset means using default one.
#  - `CPM_SOURCE_CACHE` for caching source of dependencies by CPM.cmake.

import argparse
import os
import pathlib
import platform
import shutil
import subprocess

OS_WIN = 'Windows'


# `distuils` is deprecated and will be removed since 3.12. Let's retain the
# function.
def strtobool(val):
    """Convert a string representation of truth to true (1) or false (0).
    True values are 'y', 'yes', 't', 'true', 'on', and '1'; false values
    are 'n', 'no', 'f', 'false', 'off', and '0'.  Raises ValueError if
    'val' is anything else.
    """
    val = val.lower()
    if val in ('y', 'yes', 't', 'true', 'on', '1'):
        return 1
    elif val in ('n', 'no', 'f', 'false', 'off', '0'):
        return 0
    else:
        raise ValueError("invalid truth value %r" % (val,))


def default_generator():
    """
    Empty string means using the default generator of CMake; and on Windows,
    it's not easy to determine which version of Visual Studio is installed, at
    least for us, thus we let CMake make the decision.

    On other platforms, we prefer ninja over traditional makefile.
    """
    if platform.system() == OS_WIN:
        return ''

    if shutil.which('ninja'):
        return 'Ninja'
    else:
        return 'Unix Makefiles'


def selected_generator():
    return os.getenv('GENERATOR', default_generator())


def run_as_shell(args):
    cmd = ' '.join(args)
    print(cmd)
    subprocess.run(cmd, shell=True)


class uniconf_subsystem():
    def __init__(self, params):
        self._gen = params['generator']
        self._build_type = params['build_type']

        self._src = pathlib.Path(__file__).resolve().parent

        dir_name = self._gen.strip().replace(' ', '-').lower() +\
                   '-' + self._build_type
        self._out = pathlib.Path(self._src) / 'out' / 'build' / dir_name

        self._cpm_cache = params['cpm_cache_dir']
        self._clang_tidy = params['clang_tidy']
        self._sanitizer = params['sanitizer']

    def generate(self):
        cmd = ['cmake']

        if self._cpm_cache:
            cache_dir = pathlib.Path(self._cpm_cache).resolve()
            cmd.append(f'-DCPM_SOURCE_CACHE="{cache_dir}"')

        if self._gen:
            cmd.append(f'-G "{self._gen}"')

        clang_tidy = 'ON' if self._clang_tidy else 'OFF'
        cmd.append(f'-DHTTP_MULTIPART_ENABLE_CLANG_TIDY={clang_tidy}')

        sanitizer = 'ON' if self._sanitizer else 'OFF'
        cmd.append(f'-DHTTP_MULTIPART_USE_SANITIZER={sanitizer}')

        cmd.append(f'-DCMAKE_BUILD_TYPE={self._build_type}')
        cmd.append(f'-B "{self._out}"')
        cmd.append(f'-S "{self._src}"')

        run_as_shell(cmd)

    def build(self):
        thread_num = os.cpu_count() + 2
        cmd = ['cmake',
               f'--build "{self._out}"',
               f'--verbose',
               f'-- -j {thread_num}']

        run_as_shell(cmd)

    def clean(self):
        cmd = ['cmake',
               f'--build "{self._out}"',
               '--target clean']

        run_as_shell(cmd)


class multiconf_subsystem():
    def __init__(self, params):
        self._gen = params['generator']
        self._build_type = params['build_type']

        self._src = pathlib.Path(__file__).resolve().parent

        normed_gen = self._gen.strip().replace(' ', '-').lower()
        if normed_gen == '' and platform.system() == OS_WIN:
            normed_gen = 'msvc'
        self._out = pathlib.Path(self._src) / 'out' / 'build' / normed_gen

        self._cpm_cache = params['cpm_cache_dir']
        self._clang_tidy = params['clang_tidy']
        self._sanitizer = params['sanitizer']

    def generate(self):
        cmd = ['cmake']

        if self._cpm_cache:
            cache_dir = pathlib.Path(self._cpm_cache).resolve()
            cmd.append(f'-DCPM_SOURCE_CACHE="{cache_dir}"')

        if self._gen:
            cmd.append(f'-G "{self._gen}"')

        clang_tidy = 'ON' if self._clang_tidy else 'OFF'
        cmd.append(f'-DHTTP_MULTIPART_ENABLE_CLANG_TIDY={clang_tidy}')

        cmd.append(f'-B "{self._out}"')
        cmd.append(f'-S "{self._src}"')

        run_as_shell(cmd)

    def build(self):
        cmd = ['cmake',
               f'--build "{self._out}"',
               f'--verbose',
               f'--config {self._build_type}']

        run_as_shell(cmd)

    def clean(self):
        cmd = ['cmake',
               f'--build "{self._out}"',
               f'--config {self._build_type}',
               '--target clean']

        run_as_shell(cmd)


def create_build_subsystem(params):
    generator = params['generator']

    if generator == '' and platform.system() == OS_WIN:
        return multiconf_subsystem(params)

    if generator.startswith('Visual Studio'):
        return multiconf_subsystem(params)

    return uniconf_subsystem(params)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--build-type', type=str, action='store',
                        dest='build_type', default='Release',
                        help='specify the build type; Release is by default')
    parser.add_argument('--no-build', action='store_true',
                        dest='skip_build', default=False,
                        help='generate only')
    parser.add_argument('--clean', action='store_true', dest='clean_mode',
                        default=False, help='clean up the build (will be '
                        'ignored if --no-build is specified)')
    parser.add_argument('--clang-tidy', dest='clang_tidy',
                        type=lambda opt: bool(strtobool(opt)), default=True,
                        help='enable clang-tidy on build')
    parser.add_argument('--sanitizer', dest='sanitizer',
                        type=lambda opt: bool(strtobool(opt)), default=True,
                        help='enable asan & ubsan')
    args = parser.parse_args()

    # Setup params
    params = {'generator': selected_generator(),
              'build_type': args.build_type,
              'cpm_cache_dir': os.getenv('CPM_SOURCE_CACHE'),
              'skip_build': args.skip_build,
              'clean_mode': args.clean_mode,
              'clang_tidy': args.clang_tidy,
              'sanitizer': args.sanitizer,}

    subsys = create_build_subsystem(params)

    if params['clean_mode']:
        if params['skip_build']:
            print('[*] --clean is ignored because --no-build is specified')
        else:
            subsys.clean()
            return

    # Generate or build.

    subsys.generate()

    if params['skip_build']:
        print('[*] Generate project only, skip build')
        return

    subsys.build()


if __name__ == '__main__':
    main()
