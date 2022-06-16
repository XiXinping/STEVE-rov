const live_image = document.getElementById("live-image");
const temperature_tag = document.getElementById("temperature-tag");
const x_accel_tag = document.getElementById("x-accel-tag");
const y_accel_tag = document.getElementById("y-accel-tag");
const z_accel_tag = document.getElementById("z-accel-tag");


ws_uri = "ws://192.168.100.1:8765";

var main_ws = new WebSocket(ws_uri);

// tell server what kind of client this is
const main_client_info = {'client_type': 'web_client_main'};
main_ws.addEventListener('open', (event) => {
    main_ws.send(JSON.stringify(main_client_info));
});
main_ws.onmessage = function (event) {
    var sensor_data = JSON.parse(event.data);
    console.log(`Sensor Data: ${sensor_data}`);

    temperature_tag.innerText = `${sensor_data.t}°C`;
    x_accel_tag.innerText = `${sensor_data.xa}m/s^2`;
    y_accel_tag.innerText = `${sensor_data.ya}m/s^2`;
    z_accel_tag.innerText = `${sensor_data.za}m/s^2`;
};
