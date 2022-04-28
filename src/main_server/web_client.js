var leak_tag = document.getElementById("leak-tag");
var temperature_tag = document.getElementById("temperature-tag");
var current_tag = document.getElementById("current-tag");
const live_image = document.getElementById("live-image");
const alarm = document.getElementById("alarm-tag");

document.querySelector("#live-image").style.transform = 
    "rotate(180deg)";


ws_uri = "ws://192.168.0.213:8765";

var main_ws = new WebSocket(ws_uri);

// tell server what kind of client this is
const main_client_info = {'client_type': 'web_client_main'};
main_ws.addEventListener('open', (event) => {
    main_ws.send(JSON.stringify(main_client_info));
});
main_ws.onmessage = function (event) {
    var sensor_data = JSON.parse(event.data);
    if (sensor_data.leak_sensor == true) {
        audio.play();
        leak_tag.innerText = 'Leak Sensor: LEAK!!! AAAAAHHHHH!!!';
    } else {
        leak_tag.innerText = 'Leak Sensor: No Leak';
    }
    temperature_tag.innerText = `Temperature:\
    ${sensor_data.temperature}°C`;
    current_tag.innerText = `Current: ${sensor_data.current}A`;
};

var camera_ws = new WebSocket(ws_uri);

camera_ws.binaryType = 'arraybuffer';
const camera_client_info = {'client_type': 'web_client_camera'};
camera_ws.addEventListener('open', (event) =>{
    camera_ws.send(JSON.stringify(camera_client_info));
});
camera_ws.onmessage = function (event) {
    var arrayBuffer = event.data;
    var blob  = new Blob([new Int8Array(arrayBuffer)], {type: "image/jpg"});
    live_image.src = window.URL.createObjectURL(blob);
};

