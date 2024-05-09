# node-libcmt

This is a JavaScript/TypeScript library for interacting with Cartesi Rollups.

## Installation

```shell
npm install @tuler/node-libcmt
# yarn add @tuler/node-libcmt
# pnpm add @tuler/node-libcmt
```

## How to use

```typescript
import { Rollups } from "@tuler/node-libcmt";

// creating a rollups instance
const rollups = new Rollups();

// request
let req = rollups.finish(true);
while (req) {
    // process the request
    if (req.type === "advance") {
        const index = rollups.emit_voucher({
            destination: "0x491604c0fdf08347dd1fa4ee062a822a5dd06b5d",
            value: 0n,
            payload: "0x",
        });
        console.log(`created voucher ${index} to transfer CTSI`);
    } else if (req.type === "inspect")

    }

    // finish the request
    req = rollups.finish(true);
}
```

## TypeScript

The library contains TypeScript type definitions.

## Building

The binding is built in two layers.

The C++ layer uses [node-addon-api](https://github.com/nodejs/node-addon-api) and exposes 95% of the Cartesi Machine C API.

Then there is a TypeScript layer which exposes a limited and possibly more user friendly API.

## License

This binding code is license under Apache-2 license.

The machine-emulator repository and all contributions to it are licensed under the LGPL 3.0, unless otherwise specified below or in subdirectory LICENSE / COPYING files. Please review our COPYING file for the LGPL 3.0 license.

## Additional Notes

This project may include or link to other software components with different licenses. Contributors and users are responsible for ensuring compliance with each component's licensing terms. For detailed information, please refer to the individual LICENSE files within each directory or submodule, and for the Debian packages, please review the respective copyright and licensing details as mentioned above.
