# Contribution Guuide

### Coding Style

Setup git hooks to enforce Google C++ style with clang-format

```bash
  git config core.hooksPath .githooks
  chmod +x .githooks/pre-commit
```

Use clang-tidy to enforce coding style and best practices

- Compile with `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` to generate compile_commands.json
- Run clang-tidy `clang-tidy /path/to/files -p=/path/to/compile_commands.json --extra-arg=-std=c++17`
- Review suggested changes and use `-fix` option if available