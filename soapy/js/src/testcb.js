// test.js
const addon = require('../build/Release/vectorops.node');

addon((msg) => {
  console.log(msg);
// Prints: 'hello world'
});

