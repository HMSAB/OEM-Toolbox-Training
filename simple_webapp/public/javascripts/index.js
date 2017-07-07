$(document).ready(function () {
  var timeData = [],
    outputfreq = [],
    outputvoltage = [];
  var data = {
    labels: timeData,
    datasets: [
      {
        fill: false,
        label: 'Frequency (Hz)',
        yAxisID: 'Frequency',
        borderColor: "rgba(255, 204, 0, 1)",
        pointBoarderColor: "rgba(255, 204, 0, 1)",
        backgroundColor: "rgba(255, 204, 0, 0.4)",
        pointHoverBackgroundColor: "rgba(255, 204, 0, 1)",
        pointHoverBorderColor: "rgba(255, 204, 0, 1)",
        data: outputfreq
      },
      {
        fill: false,
        label: 'Voltage',
        yAxisID: 'Voltage',
        borderColor: "rgba(24, 120, 240, 1)",
        pointBoarderColor: "rgba(24, 120, 240, 1)",
        backgroundColor: "rgba(24, 120, 240, 0.4)",
        pointHoverBackgroundColor: "rgba(24, 120, 240, 1)",
        pointHoverBorderColor: "rgba(24, 120, 240, 1)",
        data: outputvoltage
      }
    ]
  }

  var basicOption = {
    title: {
      display: true,
      text: 'Voltage & Frequency Real-time Data',
      fontSize: 36
    },
    scales: {
      yAxes: [{
        id: 'Frequency',
        type: 'linear',
        scaleLabel: {
          labelString: 'Frequency(Hz)',
          display: true
        },
        position: 'left',
      }, {
          id: 'Voltage',
          type: 'linear',
          scaleLabel: {
            labelString: 'Volatage(V)',
            display: true
          },
          position: 'right'
        }]
    }
  }

  //Get the context of the canvas element we want to select
  var ctx = document.getElementById("myChart").getContext("2d");
  var optionsNoAnimation = { animation: false }
  var myLineChart = new Chart(ctx, {
    type: 'line',
    data: data,
    options: basicOption
  });

  var ws = new WebSocket('wss://' + location.host);
  ws.onopen = function () {
    console.log('Successfully connect WebSocket');
  }
  ws.onmessage = function (message) {
    console.log('receive message' + message.data);
    try {
      var obj = JSON.parse(message.data);
      if(!obj.time || !obj.outputvoltage) {
        return;
      }
      timeData.push(obj.time);
      outputvoltage.push(obj.outputvoltage);
      // only keep no more than 50 points in the line chart
      const maxLen = 50;
      var len = timeData.length;
      if (len > maxLen) {
        timeData.shift();
        outputvoltage.shift();
      }

      if (obj.outputfreq) {
        outputfreq.push(obj.outputfreq);
      }
      if (outputfreq.length > maxLen) {
        outputfreq.shift();
      }

      myLineChart.update();
    } catch (err) {
      console.error(err);
    }
  }
});
