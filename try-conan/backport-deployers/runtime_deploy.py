# runtime_deploy.py
import os
from conan.tools.files import copy

def deploy(graph, output_folder, **kwargs):
    conanfile = graph.root.conanfile
    copied = set()

    def _copy_patterns(src_dir, patterns):
        if not src_dir or not os.path.isdir(src_dir):
            return
        for pattern in patterns:
            copy(conanfile, pattern, src=src_dir, dst=output_folder)
            copied.add((src_dir, pattern))

    for dep in conanfile.dependencies.values():
        pkg_folder = getattr(dep, "package_folder", None)
        if not pkg_folder:
            continue

        cpp_info = getattr(dep, "cpp_info", None)
        if cpp_info is None:
            continue

        bindirs = list(getattr(cpp_info, "bindirs", []) or [])
        libdirs = list(getattr(cpp_info, "libdirs", []) or [])

        for bindir in bindirs:
            abs_bindir = bindir if os.path.isabs(bindir) else os.path.join(pkg_folder, bindir)
            _copy_patterns(abs_bindir, ["*.dll", "*.exe", "*.dylib", "*.so", "*.so.*"])

        for libdir in libdirs:
            abs_libdir = libdir if os.path.isabs(libdir) else os.path.join(pkg_folder, libdir)
            _copy_patterns(abs_libdir, ["*.dll", "*.dylib", "*.so", "*.so.*"])

    conanfile.output.info(f"Runtime artifacts deployed to: {output_folder}")
