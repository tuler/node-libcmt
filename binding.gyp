{
    "targets": [
        {
            "target_name": "node_libcmt",
            "sources": [
                "src/binding.cc",
                "src/rollup.cc",
            ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include_dir\")",
                "<(module_root_dir)/../machine-emulator-tools/sys-utils/libcmt/_install/include",
                "/usr/riscv64-linux-gnu/include",
            ],
            "dependencies": [
                "<!(node -p \"require('node-addon-api').targets\"):node_addon_api",
            ],
            "link_settings": {
                "libraries": [
                    "-Wl,-rpath,@loader_path",
                    "-L<(module_root_dir)/../machine-emulator-tools/sys-utils/libcmt/_install/lib",
                    "-lcmt"
                ],
            },
            "defines": [],
        },
        {
            "target_name": "action_after_build",
            "type": "none",
            "dependencies": [ "node_libcmt" ],
            "copies": [
                {
                    "files": [ "<(PRODUCT_DIR)/node_libcmt.node" ],
                    "destination": "./lib/binding/"
                }
            ]
        }
    ],
}
