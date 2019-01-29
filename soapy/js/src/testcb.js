const Benchmark = require('benchmark');
const suite = new Benchmark.Suite;
const kindOf = require('kind-of');

// grab our module
const addon = require('../build/Release/vectorops.node');


function test_async() {
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
}


function test_callback() {
  addon.runCallback((msg) => {
    console.log(msg);
  // Prints: 'hello world'
  });
}


function test_get_buffer() {
  let res = addon.GetBuffer();

  // this.thebuffer = new ArrayBuffer(this.chunk*8*2); // in bytes
  // this.uint8_view = new Uint8Array(this.thebuffer);
  uint32_view = new Uint32Array(res.buffer);


  console.log("uint32 view:");
  console.log(uint32_view[0].toString(16));
  console.log(uint32_view[1]);
  // this.float64_view = new Float64Array(this.thebuffer);

  // console.log(kindOf(res));
}

function test_transform_buffer() {

  let buffer = Buffer.alloc(1024*4);
  // buffer[0] = 'a';
  // buffer[1] = 'b';
  // buffer[2] = 'c';
  // buffer[3] = 'd';
  // buffer[4] = 0;
  // buffer[5] = 0;
  // buffer[6] = 0;
  // buffer[7] = 0;
  buffer.write("AbCdadklfjasfasdflkjasdflkasdjf,xnclx423423423lkaslaksfasf,mnasd");

  console.log(buffer.length);

  // console.log(buffer.slice(0,8));

  let res = addon.TransformBuffer(buffer, buffer.length);

  console.log(res.slice(0,8));

  uint32_view = new Uint32Array(res.buffer);


  console.log("uint32 view:");
  console.log(uint32_view[0].toString(16));
  // console.log(uint32_view[1]);
}



function run_bench() {

// suite.add('Deferred test', function(deferred) {
//     // call resolve() when the deferred test is finished
//     deferred.resolve();
// }, {'defer': true});


  let tranformBuffer = Buffer.alloc(1024*4);
  tranformBuffer.write("abcdadklfjasfasdflkjasdflkasdjf,xnclx423423423lkaslaksfasf,mnasd");



  // add tests
  suite.add('Get 1024 uint Buffer', function() {
    let res = addon.GetBuffer();
  })
  .add('Transform 1024 uint Buffer', function() {
    let res = addon.TransformBuffer(tranformBuffer, tranformBuffer.length);
  })

  // add listeners
  .on('cycle', function(event) {
    console.log(String(event.target));
  })
  .on('complete', function() {
    console.log('Fastest is ' + this.filter('fastest').map('name'));
  })
  // run async
  .run({ 'async': true });

}
 















// test_async();
// test_callback();

// test_get_buffer();
test_transform_buffer();
// run_bench();
