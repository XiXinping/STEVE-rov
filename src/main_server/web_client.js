const live_image = document.getElementById("live-image");

document.querySelector("#live-image").style.transform = 
    "rotate(180deg)";


ws_uri = "ws://10.0.0.27:8765";

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

