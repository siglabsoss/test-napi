// test.js
const addon = require('../build/Release/smodem.node');

addon((msg) => {
  console.log(msg);
// Prints: 'hello world'
});

