# Testing

Currently testing is made with a small tool called `ckester` I made for this project. This tool expects a `test/suite` directory at the root of the project. When running `make` for this tool with the makefile in it's root dir, it will generate it's binary from the file `src/ckester_cli.c` relative to the root of that project (which currently is in `vendor/ckester`).

This tool has various commands like:

- `ckester all`: compiles and runs all C files on the `test/suite` dir relative to `pwd`.
- `ckester [unit|integration|system]`: compiles and runs `test/suite/[unit|integration|system]` based on the argument.
- `ckester single {filename|relative_filename}`: compiles and runs a single test, you can give the file name of the C script or a relative dir name (need to work on this feature because it has logic problems of same name files).

This sort of commands are according to what I like in a workflow with tests; which is to run tests based on their level.

When running the `makefile` inside `ckester`, it will also append an include statement into the `pwd` makefile. This will allow to execute some make targets which are wrappers for the commands. You can check them out by running:

```bash
$ make test-help
```
