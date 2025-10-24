#!/usr/bin/env python3
"""
clang_tidy_wrapper.py — A wrapper to filter unsupported compiler flags
before running clang-tidy.

Usage: clang_tidy_wrapper.py [clang-tidy args...]
"""

import os
import sys
import subprocess
from shutil import which

# Blacklist of flags that Clang-Tidy / Clang doesn't understand
UNSUPPORTED_FLAGS = {
    "-mno-direct-extern-access",
    "-fstack-protector-strong",
    "-fno-plt",
    "-mfunction-return=thunk-extern",
    "-mindirect-branch=thunk-extern",
    "-mrecord-mcount",
}

def filter_flags(args):
    """Remove any unsupported compiler flags from argument list."""
    filtered = []
    for arg in args:
        if any(bad in arg for bad in UNSUPPORTED_FLAGS):
            continue
        filtered.append(arg)
    return filtered

def main():
    clang_tidy = which("clang-tidy")
    if not clang_tidy:
        print("Error: clang-tidy not found in PATH.", file=sys.stderr)
        sys.exit(1)

    # Filter unsupported flags from the command line
    filtered_args = filter_flags(sys.argv[1:])

    # Run clang-tidy with the cleaned-up arguments
    result = subprocess.run([clang_tidy] + filtered_args)
    sys.exit(result.returncode)

if __name__ == "__main__":
    main()
