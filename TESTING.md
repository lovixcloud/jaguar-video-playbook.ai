# Jaguar Testing Documentation

## Unit & Integration Tests

The test suite is compiled into `JaguarTests.exe`.

### Test Coverage

1. **TestProjectModel:** Tests JSON serialization, project loading, saving, and format integrity.
2. **TestTimelineOperations:** Tests multi-track clip addition, movement, split, trim, and removal.
3. **TestUndoRedo:** Tests command history execution, undo, redo, and stack management.
4. **TestMediaCacheAndWaveform:** Tests disk cache storage, retrieval, and background peak processing.
5. **TestExportQueue:** Tests asynchronous export job initialization, progress calculation, and queue handling.

Run tests:
```bash
build/JaguarTests.exe
```
