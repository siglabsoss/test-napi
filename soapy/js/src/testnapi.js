
const addon = require('../build/Release/smodem.node');
const assert = require('assert');

addon.RunCallback(function(msg) {
    console.log(msg);
  // assert.strictEqual(msg, 'hello world');
});

function testRecv(desiredRecv) {
  addon.RunCallbackWithRecv(function() {
    // assert.strictEqual(this, desiredRecv);
  }, desiredRecv);
}

testRecv(undefined);
testRecv(null);
testRecv(5);
testRecv(true);
testRecv('Hello');
testRecv([]);
testRecv({});