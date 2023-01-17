
```sh
git submodule update --init
```

### Compiling the Source

Once you have the repository and the submodules (libDaisy/DaisySP) properly cloned, and the toolchain installed (for details see the [Daisy Wiki](https://github.com/electro-smith/DaisyWiki/wiki) for platform specific instructions) it's time to build the libraries, and some examples.

To build both libraries at once simply run:

`./scripts/build_libs.sh`

This is the same as going to each library's directory and running `make`.