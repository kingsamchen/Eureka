#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import pathlib
import os
import re

SRC_EXT = {'.cc', '.c', '.h'}
ERR_TYPES = {
    "ABORTED": "AbortedError",
    "ALREADY_EXISTS": "AlreadyExistsError",
    "CANCELLED": "CancelledError",
    "DATA_LOSS": "DataLossError",
    "DEADLINE_EXCEEDED": "DeadlineExceededError",
    "FAILED_PRECONDITION": "FailedPreconditionError",
    "INTERNAL": "InternalError",
    "INVALID_ARGUMENT": "InvalidArgumentError",
    "NOT_FOUND": "NotFoundError",
    "OUT_OF_RANGE": "OutOfRangeError",
    "PERMISSION_DENIED": "PermissionDeniedError",
    "RESOURCE_EXHAUSTED": "ResourceExhaustedError",
    "UNAUTHENTICATED": "UnauthenticatedError",
    "UNAVAILABLE": "UnavailableError",
    "UNIMPLEMENTED": "UnimplementedError",
    "UNKNOWN": "UnknownError"
}

ERR_IS = {
    "ABORTED": "IsAborted",
    "ALREADY_EXISTS": "IsAlreadyExists",
    "CANCELLED": "IsCancelled",
    "DATA_LOSS": "IsDataLoss",
    "DEADLINE_EXCEEDED": "IsDeadlineExceeded",
    "FAILED_PRECONDITION": "IsFailedPrecondition",
    "INTERNAL": "IsInternal",
    "INVALID_ARGUMENT": "IsInvalidArgument",
    "NOT_FOUND": "IsNotFound",
    "OUT_OF_RANGE": "IsOutOfRange",
    "PERMISSION_DENIED": "IsPermissionDenied",
    "RESOURCE_EXHAUSTED": "IsResourceExhausted",
    "UNAUTHENTICATED": "IsUnauthenticated",
    "UNAVAILABLE": "IsUnavailable",
    "UNIMPLEMENTED": "IsUnimplemented",
    "UNKNOWN": "IsUnknown"
}

def replace_case_1(data):
    """
    can handle

        auto s1 = Status(INVALID_ARGUMENT, "err msg");

        auto s2 = Status(ErrorCode::INVALID_ARGUMENT, "err msg" + ex.what);

        auto s3 = Status(INVALID_ARGUMENT, std::string("err msg") + ex.what);

        auto s4 = Status(ErrorCode::INVALID_ARGUMENT, ErrMsg::Errmsg + ex.what);

        auto s5 = Status(INVALID_ARGUMENT,
                         ErrMsg::Errmsg +
                         ex.what);
    """
    PREFIX = 'Status('
    pos = 0
    while True:
        pos = data.find(PREFIX, pos)
        if pos == -1:
            return data

        start = pos + len(PREFIX)

        if data[pos-1].isalpha():
            # hit str like collectStatus(
            pos = start
            continue

        paren = 1
        cnt = 0
        for ch in data[start:]:
            cnt += 1
            if ch == '(':
                paren += 1
            elif ch == ')':
                paren -= 1
                if paren == 0:
                    break
        if cnt == 1:
            pos = start + 1
            continue

        end = start + cnt - 1   # point to `)`
        params = list(map(lambda p: p.strip(), data[start:end].split(',', 1)))
        params[0] = params[0].split(':')[-1]
        try:
            new_str = format(f'absl::{ERR_TYPES[params[0]]}({params[1]})')
        except KeyError as e:
            print(f' -> Skip unexpected error code: {e}')
            pos = end
            continue
        data = data[:pos] + new_str + data[end + 1:]
        pos += len(new_str)


def replace_case_2(data):
    """
    can handle forms like

        {ErrorCode::INVALID_ARGUMENT, "err msg" + ex.what}

        {ErrCode::INVALID_ARGUMENT, "err msg" + ex.what}

        {Code::INVALID_ARGUMENT, "err msg" + ex.what}
    """
    CASE_PREFIXES = ('{ErrorCode::', '{ErrCode::', '{Code::', '{error::Code::',)
    def _replace_internal(data, prefix):
        pos = 0
        while True:
            pos = data.find(prefix, pos)
            if pos == -1:
                return data

            paren = 1
            cnt = 0
            start = pos + len(prefix)
            for ch in data[start:]:
                cnt += 1
                if ch == '{':
                    paren += 1
                elif ch == '}':
                    paren -= 1
                    if paren == 0:
                        break
            # WTF is {ErrorCode::ABORTED} ??? ctor should require 2 arguments.
            if cnt == 1:
                pos = start + 1
                continue

            end = start + cnt - 1
            params = list(
                map(lambda p: p.strip(), data[start:end].split(',', 1)))
            params[0] = params[0].split(':')[-1]
            try:
                new_str = format(f'absl::{ERR_TYPES[params[0]]}({params[1]})')
            except KeyError as e:
                print(f' -> Skipped unexpected error code: {e}')
                pos = end
                continue
            data = data[:pos] + new_str + data[start + cnt:]
            pos + len(new_str)

    for pre in CASE_PREFIXES:
        data = _replace_internal(data, pre)

    return data


def replace_case_3(data):
    """
    can handle forms like
        Status status(ErrorCode::INTERNAL, Consts::STR_INTERNAL_DB_ERR)

        Status s(ErrorCode::INTERNAL, Consts::STR_INTERNAL_DB_ERR)
    """
    CASE_PREFIXES = ('Status status(', 'Status s(',)
    def _replace_internal(data, prefix):
        pos = 0
        while True:
            pos = data.find(prefix, pos)
            if pos == -1:
                return data

            start = pos + len(prefix)
            if data[pos - 1] != ' ':
                print(f' -> suspicious {data[pos-1:start]}')
                pos = start
                continue

            paren = 1
            cnt = 0
            for ch in data[start:]:
                cnt += 1
                if ch == '(':
                    paren += 1
                elif ch == ')':
                    paren -= 1
                    if paren == 0:
                        break
            # WTF is Status s(), most vexing parse error
            if cnt == 1:
                pos = start + 1
                continue

            end = start + cnt - 1   # points to matched `)`
            params = list(
                map(lambda p: p.strip(), data[start:end].split(',', 1)))
            params[0] = params[0].split(':')[-1]
            var_name = data[pos:start].split(' ')[-1].rstrip('(')
            try:
                new_str = format(f'auto {var_name} = absl::{ERR_TYPES[params[0]]}({params[1]})')
            except KeyError as e:
                print(f' -> Skipped unexpected error code: {e}')
                pos = end
                continue
            data = data[:pos] + new_str + data[end + 1:]
            pos + len(new_str)

    for pre in CASE_PREFIXES:
        data = _replace_internal(data, pre)

    return data


def replace_case_4(data):
  def __ut_match_helper(m):
    prefix = m.group('prefix')
    ast_type = m.group('ast_type')
    err_code = m.group('err_code')
    status = m.group('status')
    suffix = m.group('suffix')
    rep_line = '{}{}_TRUE(absl::{}({})){}'.format(prefix, ast_type, ERR_IS[err_code], status, suffix)
    # print("{} {} {}".format(ast_type, err_code, status))
    print('{} => {}'.format(line, rep_line))
    return rep_line
  def __if_match_helper(m):
    prefix = m.group('prefix')
    eq_type = m.group('eq')
    err_code = m.group('err_code')
    status = m.group('status')
    suffix = m.group('suffix')
    not_sign = ''
    if eq_type == '!=':
      not_sign = '!'
    rep_line = '{}{}absl::{}({}){}'.format(prefix, not_sign, ERR_IS[err_code], status, suffix)
    # print("{} {} {}".format(ast_type, err_code, status))
    print('{} => {}'.format(line, rep_line))
    return rep_line
  ret = ''
  lines = data.splitlines()
  err_code_pattern = '\w+::(?P<err_code>ABORTED|ALREADY_EXISTS|CANCELLED|DATA_LOSS|DEADLINE_EXCEEDED|FAILED_PRECONDITION|INTERNAL|INVALID_ARGUMENT|NOT_FOUND|OUT_OF_RANGE|PERMISSION_DENIED|RESOURCE_EXHAUSTED|UNAUTHENTICATED|UNAVAILABLE|UNIMPLEMENTED|UNKNOWN)'
  status_pattern = '(?P<status>\w+).(error_code|code)\(\)'
  pattern = '(?P<prefix>\s*)(?P<ast_type>ASSERT|EXPECT)_EQ\({},\s{}\)(?P<suffix>.*)'.format(err_code_pattern, status_pattern)
  # print(pattern)
  p1 = re.compile(pattern)
  p2 = re.compile('(?P<prefix>\s*)(?P<ast_type>ASSERT|EXPECT)_EQ\({},\s{}\)(?P<suffix>.*)'.format(status_pattern, err_code_pattern))
  p3 = re.compile('(?P<prefix>.*?){}\s(?P<eq>==|!=)\s{}(?P<suffix>.*)'.format(status_pattern, err_code_pattern))
  p4 = re.compile('(?P<prefix>.*?){}\s(?P<eq>==|!=)\s{}(?P<suffix>.*)'.format(err_code_pattern, status_pattern))
  p5 = re.compile('using\s\w+\s=\sgoogle::protobuf::util::error::Code;')
  for line in lines:
    if p5.match(line):
      continue
    m = p1.match(line)
    if m:
      ret = ret + __ut_match_helper(m) + '\n'
      continue
    m = p2.match(line)
    if m:
      ret = ret + __ut_match_helper(m) + '\n'
      continue
    m = p3.match(line)
    if m:
      ret = ret + __if_match_helper(m) + '\n'
      continue
    ret = ret + line + '\n'
  return ret


def replace_case_5(data: str):
    data = data.replace('.error_message().ToString()', '.ToString()')
    data = data.replace('.error_message()', '.message()')
    return data


def do_replace(fp):
    with open(fp, mode='r+', newline='\n') as f:
        data = f.read()

        # Remove using
        data = data.replace(
            'using ErrorCode = google::protobuf::util::error::Code;', '')

        # Status::OK -> absl::OkStatus()
        data = data.replace('Status::OK', 'absl::OkStatus()')

        # Status(INVALID_ARGUMENT, "error msg") ->
        # absl::InvalidArgumentError("err msg")
        data = replace_case_1(data)

        # {ErrorCode::INVALID_ARGUMENT, "error msg"} ->
        # absl::InvalidArgumentError("err msg")
        data = replace_case_2(data)

        # Status status(ErrorCode::INTERNAL, Consts::STR_INTERNAL_DB_ERR)
        data = replace_case_3(data)

        # case 3: EXPECT_EQ(ErrorCode::INTERNAL, status.code()); ->
        #         EXPECT_TURE(absl::IsInternal(status));
        #         status.error_code() == ErrorCode::FAILED_PRECONDITION -?
        #         absl::IsFailedPrecondition(status)
        data = replace_case_4(data)

        # .error_message().ToString() -> .ToString()
        # .error_message() -> .message()
        data = replace_case_5(data)

        # Let's overwrite
        f.seek(0, os.SEEK_SET)
        f.write(data)
        f.truncate()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('target_dir', type=str, help='path to the target dir')
    parser.add_argument('pattern', type=str, help='match pattern for files')
    args = parser.parse_args()
    files = pathlib.Path(args.target_dir).glob(args.pattern)
    for file in files:
        fp = file.resolve()
        if fp.is_dir():
            continue
        print(f'Processing {fp}')
        if fp.suffix not in SRC_EXT:
            print(' -> Skipped for not a source file')
            continue
        do_replace(fp)
        print(' -> DONE')

if __name__ == '__main__':
    main()
