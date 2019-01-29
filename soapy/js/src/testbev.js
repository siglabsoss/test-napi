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

function IShortToStreamable(iShort) {

  let sz = iShort.length;

  let thebuffer = new ArrayBuffer(sz*4); // in bytes
  let uint8_view = new Uint8Array(thebuffer);
  let typedView = new Uint32Array(thebuffer);
  for(let i = 0; i < sz; i++) {
    typedView[i] = iShort[i];
  }

  return uint8_view;
}


function test_transform_buffer() {

  let list = [];
  for(let i = 0; i < 1024; i++) {
    list.push(i);
  }

  let buffer = Buffer.from(IShortToStreamable(list));
  // console.log(buffer.length);
  // console.log(buffer.slice(0,12));

  addon.setStreamCallback(function(res) {
    console.log('stream callback');
    console.log(res);
    let uint32_view = new Uint32Array(res.buffer);
    console.log('uint32 view:');
    console.log(uint32_view[0].toString(16));
  });

  setInterval(()=>{  addon.writeStreamData(buffer, buffer.length) }, 1000);


  // addon.writeStreamData(buffer, buffer.length);
  // addon.writeStreamData(buffer, buffer.length);
  // addon.writeStreamData(buffer, buffer.length);
  // addon.writeStreamData(buffer, buffer.length);
}















// test_async();
// test_callback();

// test_get_buffer();
test_transform_buffer();
// run_bench();
