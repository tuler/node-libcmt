var binary = require("@mapbox/node-pre-gyp");
var path = require("path");
const binding_path = binary.find(
    path.resolve(path.join(__dirname, "..", "./package.json")),
);
const addon = require(binding_path);

export type Address = `0x${string}`;
export type Payload = Uint8Array;

export interface RollupFinish {
    type: "advance" | "inspect";
}

export interface RollupAdvance extends RollupFinish {
    type: "advance";
    app_contract: Address;
    block_number: bigint;
    block_timestamp: bigint;
    chain_id: bigint;
    index: bigint;
    msg_sender: Address;
    payload: Payload;
}

export interface RollupInspect extends RollupFinish {
    type: "inspect";
    payload: Payload;
}

export interface Voucher {
    destination: Address;
    value: bigint;
    payload: Payload;
}

export interface Rollup {
    finish(accept: boolean): RollupAdvance | RollupInspect;
    emit_voucher(voucher: Voucher): number;
    emit_notice(payload: Payload): number;
    emit_report(payload: Payload): void;
    emit_exception(payload: Payload): void;
    progress(value: number): void;
}

export var Rollup: {
    new (): Rollup;
} = addon.Rollup;
