const Benchmark = require('benchmark');
const suite = new Benchmark.Suite;
const kindOf = require('kind-of');
const _ = require('lodash');

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

// Takes a Buffer or Uint8Array and returns an array of Javascript numbers
function streamableToIShort(chunk) {
  const uint8_in_view = new Uint8Array(chunk, 0, chunk.length);
  let dataView = new DataView(uint8_in_view.buffer);

  // width of input in bytes
  const width = 4;

  // how many uint32's (how many samples) did we get
  const sz = chunk.length / width;

  let result = Array(sz);

  for(let i = 0; i < sz; i++) {
    let val = dataView.getUint32(i*4, true);
    result[i] = val;
  }

  return result;
}


function test_transform_buffer() {
  // setInterval(()=>{ console.log('still here') }, 333);

  let doShutdown = false;

  let list = [];
  let expected = [];
  for(let i = 0; i < 1024; i++) {
    list.push(i);
    expected.push(i*8);
  }

  let buffer = Buffer.from(IShortToStreamable(list));
  // console.log(buffer.length);
  // console.log(buffer.slice(0,12));

  addon.startStream();

  addon.setStreamCallback(function(res) {
    console.log('stream callback');
    // console.log(res);

    let same = _.isEqual(expected, streamableToIShort(res));

    console.log('same ' + same);

    // let uint32_view = new Uint32Array(res.buffer);
    // console.log('uint32 view:');
    // console.log(uint32_view[0].toString(16));
  });

  let interval = setInterval(()=>{  addon.writeStreamData(buffer, buffer.length) }, 1000);

  setTimeout(()=>{ addon.stopStream(); clearInterval(interval); }, 3000);




  // addon.writeStreamData(buffer, buffer.length);
  // addon.writeStreamData(buffer, buffer.length);
  // addon.writeStreamData(buffer, buffer.length);
  // addon.writeStreamData(buffer, buffer.length);
}


function test_transform_buffer2() {
  // setInterval(()=>{ console.log('still here') }, 333);

  let doShutdown = false;

  let list = [];
  let expected = [];
  for(let i = 0; i < 1024; i++) {
    list.push(i);
    expected.push(i*8);
  }

  let buffer = Buffer.from(IShortToStreamable(list));
  // console.log(buffer.length);
  // console.log(buffer.slice(0,12));

  addon.startStream();

  addon.setStreamCallback(function(res) {
    console.log('stream callback');
    // console.log(res);

    let same = _.isEqual(expected, streamableToIShort(res));

    console.log('same ' + same);

    // let uint32_view = new Uint32Array(res.buffer);
    // console.log('uint32 view:');
    // console.log(uint32_view[0].toString(16));
  });

  addon.writeStreamData(buffer, buffer.length);
  addon.writeStreamData(buffer, buffer.length);

  // let interval = setInterval(()=>{  addon.writeStreamData(buffer, buffer.length) }, 1000);

  setTimeout(()=>{ addon.stopStream(); }, 1000);




  // addon.writeStreamData(buffer, buffer.length);
  // addon.writeStreamData(buffer, buffer.length);
  // addon.writeStreamData(buffer, buffer.length);
  // addon.writeStreamData(buffer, buffer.length);
}



function test_torture() {

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

  let runTorture = 1;

  let torture = function() {
    for(let i = 0; i < 10000; i++) {
      addon.writeStreamData(buffer, buffer.length);
    }
    if(runTorture) {
      setImmediate(torture);
    }
  };

  torture();

  setTimeout(()=>{ runTorture = 0;  }, 1000*10);


  // addon.writeStreamData(buffer, buffer.length);
  // addon.writeStreamData(buffer, buffer.length);
  // addon.writeStreamData(buffer, buffer.length);
  // addon.writeStreamData(buffer, buffer.length);
}
















// test_async();
// test_callback();

// test_get_buffer();
// test_transform_buffer();
test_transform_buffer2();
// test_torture();
// run_bench();
