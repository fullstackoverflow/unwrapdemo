const addon = require("../build/Release/addon.node");
console.log(addon);
const { A, B } = addon;
const instanceA = new A(2);
const b = new B(instanceA);
console.log(instanceA, b.GetValue());