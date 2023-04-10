const mqtt = require('mqtt');

host = '192.168.43.192';
port = '1883';
const client = mqtt.connect(`mqtt://${host}:${port}`,{
  username:'pi',
  password:'raspberry'
});

// var topic = "temperature";

// client.on('connect', function () {
//   console.log('Connected');
// });

// client.on('message', function (topic, message) {
//   console.log('Received message:', topic, message.toString());
// });

client.subscribe('sensorDHT22',{qos:1});

client.on('message', function(topic, message) {
  if (topic === 'sensorDHT22') {
    temperature = message;
    console.log(`Iso konek meng ${topic}`)
  }
});

client.on('error', function (error) {
  console.log(error);
});

let lastTimestamp = null;

client.on('message', function (topic, message) {
  // Menampilkan data suhu pada console
  console.log(temperature.toString());
  // console.log('Kelembaban: ' + humidity.toString());
  const now = new Date()
  const timestamp = formatDate(now)
  console.log(`Timestamp: ${timestamp}`)

  if (lastTimestamp) {
    const delay = now.getTime() - lastTimestamp.getTime()
    console.log(`Delay: ${delay} ms`)
  }

  lastTimestamp = now
})



function formatDate(date) {
  const hours = padLeft(date.getHours(), 2)
  const minutes = padLeft(date.getMinutes(), 2)
  const seconds = padLeft(date.getSeconds(), 2)
  const milliseconds = padLeft(date.getMilliseconds(), 3)

  return `${hours}:${minutes}:${seconds}.${milliseconds}`
}

function padLeft(number, length) {
  let str = String(number)
  while (str.length < length) {
    str = '0' + str
  }
  return str
}
