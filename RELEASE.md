# Jaguar Release Process

To create a release distribution:

1. Build `Jaguar.exe` in `Release` configuration.
2. Build `JaguarTests.exe` and verify all tests pass.
3. Package the portable distribution directory `Jaguar-Portable-x64/`.
4. Run NSIS compiler on `JaguarSetup.nsi` to create `JaguarSetup.exe`.
