{
    "targets": [
        {
            "target_name": "<(module_name)",
            "sources": [
                "src/binding.cc",
                "src/rollup.cc",
            ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include_dir\")",
                "../machine-emulator-tools/sys-utils/libcmt/_install/include",
                "/usr/riscv64-linux-gnu/include",
            ],
            "dependencies": [
                "<!(node -p \"require('node-addon-api').targets\"):node_addon_api",
            ],
            "link_settings": {
                "libraries": [ "-Wl,-rpath,@loader_path", "../../machine-emulator-tools/sys-utils/libcmt/_install/lib/libcmt.a" ],
            },
            "defines": [],
        },
        {
            "target_name": "action_after_build",
            "type": "none",
            "dependencies": [ "<(module_name)" ],
            "copies": [
                {
                    "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
                    "destination": "<(module_path)"
                }
            ]
        }
    ],
}
