# cipher
A simple ciphering program. **Supports Unicode characters**


## antext
`antext` is a simple command-line tool for analyzing text line-by-line char-by-char.


## Ciphers
### Morse code
Used by adding `--from-morse` or `--to-morse` option. \
Morse exchange table is read from [morsefile.txt](./morsefile.txt), you may easily append your own exchange pairs, add your own morsefiles with the `--morsefile` option, or disable the default morsefile with `--morsefile nodefault`.


## Error handling
There are 5 modes of error handling that can be set with the `--error` (`-e` for short) option:
- `except` throws exception upon error
- `report` immedieatly reports an error (to `stderr`), but continues
- `report_on_exit` reports all errors on exit (to `stderr`), not when they are detected
- `report_only_count_on_exit` reports only the number of error on exit (to `stderr`)
- `ignore` ignores all errors

In all modes, except `ignore`, if any error is detected, the exit code is set to 1.
