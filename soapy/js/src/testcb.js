// test.js
const addon = require('../build/Release/vectorops.node');


locations = [0xffffffff, 0xff0000ff];

// Part 4 - calling asynchronous c++ addon
addon.calculate_results_async(locations, 
  function(err, result) {
    if (err ) {
      console.log(err);
    }
    else {
        console.log(result);
      // print_rain_results(result);
    }
    
  });



addon.runCallback((msg) => {
  console.log(msg);
// Prints: 'hello world'
});

