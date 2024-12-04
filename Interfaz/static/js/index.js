var mqtt;
var reconnectTimeout = 2000;
var host = "mqtt.eclipseprojects.io";
var port = 80;

function dispenseFood() {
    message = new Paho.MQTT.Message("dispenseFood");
    message.destinationName = "actuador";
    mqtt.send(message);
}
function dispenseWater() {
    message = new Paho.MQTT.Message("dispenseWater");
    message.destinationName = "actuador";
    mqtt.send(message);
}


$(document).ready(function () {
    console.log("index.js loaded");

    $.ajax({
        url: `/feeding-data`,
        method: "GET",
        dataType: "json",
        contentType: "application/json",
        success: function (data) {
            console.log(data);
            document.getElementById("main-div").innerHTML+= "<h2>Comida: "+data.comida+" g</h2>";
            document.getElementById("main-div").innerHTML+= "<h2>Agua: "+data.agua+" ml</h2>";
        },
    });

    function onConnect() {
        console.log("Connected");
        message = new Paho.MQTT.Message("Hello from browser");
        message.destinationName = "actuador";
        mqtt.send(message);
    }
    function MQTTconnect() {
        // console.log("connecting to "+ host +" "+ port);
        mqtt = new Paho.MQTT.Client(host, port, "clientjs");
        var options = {
            timeout: 3,
            onSuccess: onConnect,
        };
        mqtt.connect(options);
    }
    MQTTconnect();

});