const live_image = document.getElementById("live-image");
const temperature_tag = document.getElementById("temperature-tag");
const x_accel_tag = document.getElementById("x-accel-tag");
const y_accel_tag = document.getElementById("y-accel-tag");
const z_accel_tag = document.getElementById("z-accel-tag");

document.querySelector("#live-image").style.transform = 
    "rotate(180deg)";

ws_uri = "ws://192.168.100.1:8765";

var main_ws = new WebSocket(ws_uri);

// tell server what kind of client this is
const main_client_info = {'client_type': 'web_client_main'};
main_ws.addEventListener('open', (event) => {
    main_ws.send(JSON.stringify(main_client_info));
});
main_ws.onmessage = function (event) {
    var sensor_data = JSON.parse(event.data);
    if (sensor_data.leak_sensor == true) {
        leak_tag.innerText = 'Leak Sensor: LEAK!!! AAAAAHHHHH!!!';
    } else {
        leak_tag.innerText = 'Leak Sensor: No Leak';
    }
    temperature_tag.innerText = `Temperature:\
    ${sensor_data.temperature}°C`;
    current_tag.innerText = `Current: ${sensor_data.current}A`;
    x_accel_tag.innerText = `X Acceleration: ${sensor_data.x_accel}m/s^2`;
    y_accel_tag.innerText = `Y Acceleration: ${sensor_data.y_accel}m/s^2`;
    z_accel_tag.innerText = `Z Acceleration: ${sensor_data.z_accel}m/s^2`;
};
