# srcVita

Baseline PS Vita app: thin wrappers over vitaGL and VitaSDK primitives. The final VPK is built from this folder. Game assets are bundled later from the gitignored `extracted/` tree. Nothing from the ROM or remaster files lives here.

## Primitives

| Primitive | Files | DS boundary it replaces | Status |
|---|---|---|---|
| renderer | `include/renderer.h`, `src/renderer.c` | The DS 2D/3D draw path. The DS call names are not yet identified in `src/`; the boundary is described by role. | built |
| input | `include/input.h`, `src/input.c` | The DS pad read the game polls each frame. Call names not yet identified in `src/`. | built |
| timing | `include/timing.h`, `src/timing.c` | The DS frame and tick timing used for fixed-rate logic. Call names not yet identified in `src/`. | built |
| file | `include/file.h`, `src/file.c` | The DS file read used to load game data. Path prefix is `FILE_APP_PREFIX` (`app0:/`). Call names not yet identified in `src/`. | built |
| memory | `include/memory.h`, `src/memory.c` | The DS allocator `func_0202f624(size)`, which callers check for NULL (see `src/base/alloc_38.c`). | built |
| audio | `include/audio.h`, `src/audio.c` | The DS sound driver. Call names not yet identified in `src/`. | stub |

`built` means the source compiles under host syntax checks with stub Vita headers. No build has run against VitaSDK yet, and nothing has run on a device.

## Build

Requires VitaSDK with `$VITASDK/bin` on `PATH`.

```bash
VITASDK=/path/to/vitasdk PATH="$VITASDK/bin:$PATH" ./build.sh
```

`build.sh` runs `make` and prints the path of `build/ghosttrick.vpk`. `param.sfo` is generated into `build/` by `vita-mksfoex` from the title and title ID in the `Makefile`. The title ID `GHTR00001` is a placeholder.

## Notes

- `src/main.c` draws a checker texture, one coloured triangle and one textured quad, then exits on START.
- The texture comes from a generated buffer, not a file.
- The heap size in `src/memory.c` is set by `_newlib_heap_size_user`. The value is a placeholder.
