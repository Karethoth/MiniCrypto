# OpenSSL for Windows
## Dependencies
- Visual Studio. Tested with VS 2022
- Perl and NASM, install them and add nasm.exe's directory to %PATH%
- Perl
  - https://strawberryperl.com/download/5.32.1.1/strawberry-perl-5.32.1.1-64bit.msi
- NASM
  - https://www.nasm.us/pub/nasm/releasebuilds/2.15.05/win64/nasm-2.15.05-installer-x64.exe
- If you haven't already, run `install-dependencies-win.sh` before proceeding

## Compiling
- Open **x64 Native Tools Command Prompt** as Administrator
- Make sure `perl -v` and `nasm -v` both run without a problem
- Navigate to <project_directory>/3rdparty/openssl
- Execute:
```
perl Configure VC-WIN64A
nmake
```

### In case of error messages about a mismatch of x86 and x64 
- Run `nmake clean`
- Open the correct command prompt as Administrator. Make sure it mentions `x64` in the title
- Retry `nmake`

