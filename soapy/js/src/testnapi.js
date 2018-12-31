const smodem = require('../build/Release/smodem.node');

let c = smodem.foo(1,4);
console.log(c);

if(smodem.doubleit(7) != 14) throw("");

if(smodem.halfit(10) != 5) throw("");

smodem.debug();
