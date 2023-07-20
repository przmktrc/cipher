# cipher
A simple ciphering program. **Supports Unicode characters**


## antext
`antext` is a simple command-line tool for analyzing text line-by-line char-by-char.


## Ciphers
### Morse code
Used by specifying `--from-morse` or `--to-morse` option.

Morse exchange table is read from [morsefile.txt](./morsefile.txt), you may easily append your own exchange pairs, add your own morsefiles with the `--morsefile` option, or disable the default morsefile with `--morsefile nodefault`.

### Exchange cipher
Used by specifying `--exchange <keyphrase>`.

Exchange cipher groups letters in the keyphrase in pairs, and when encrypting/decrypting, it swaps every occurence of a given letter with the other letter from its' pair. If a letter does not appear in the keyphrase, it is left unchanged.

This cipher is symetrical; to get the original message back, just run it again through the cipher with the same keyphrase.

**Note: Keyphrase has to be of even length with no repeating characters.**

<details>
<summary>See here for an example of how an exchange cipher works</summary>

---

Suppose we chose a keyphrase "abcdef".

The exchange pairs would be: a-b, c-d, e-f.

The message "This is a secret message" would get translated to "This is b sfdrft mfssbgf".

Since the exchange cipher is symetrical, if we encrypt that encrypted message again, we'll get the original "This is a secret message".

---

</details>


## Error handling
There are 5 modes of error handling that can be set with the `--error` (`-e` for short) option:
- `except` throws exception upon error
- `report` immedieatly reports an error (to `stderr`), but continues>
- `report_on_exit` reports all errors on exit (to `stderr`), not when they are detected
- `report_only_count_on_exit` reports only the number of error on exit (to `stderr`)
- `ignore` ignores all errors

In all modes, except `ignore`, if any error is detected, the exit code is set to 1.
