const char PAGINA_PRINCIPAL[] = 
R"=====( 
<!DOCTYPE html><html><style>header{background-color:#08a1c7;position:relative;height:50px}header h1{font-family:monospace;margin:0;position:absolute;top:50%;left:50%;transform:translate(-50%,-50%)}.menu ul{display:flex;padding:0;margin:0;list-style:none}.menu a{display:block;padding:1em;background-color:#08a1c7;text-decoration:none;color:#191c26}.menu a:hover{background-color:#036988}.menu ul li ul{display:none;position:absolute;top:100%;left:0;background-color:#08a1c7;padding:0;margin:0;list-style:none;width:100px}.menu ul li ul li a{padding:1em;background-color:#08a1c7;width:100%}.menu ul li a:hover+ul,.menu ul li ul:hover{display:block}a,p{font-size:large;font-family:monospace}h1{font-family:monospace}.titulo{margin-left:10px}canvas{max-width:700px;max-height:300px}body{max-width:50%;margin-left:25%}</style><script src="https://cdn.jsdelivr.net/npm/chart.js"></script><header><nav class="menu"><ul><li><a href="#">Menu</a><ul><li><a href="#1">Inicio</a></li><li><a href="#2">Control</a></li><li><a href="#3">Carpeta tecnica</a></li></ul></li></ul></nav><h1>ESP CALIDAD DEL AIRE</h1></header><body><h1 href="#1" class="titulo">Inicio</h1><p>asdasdasdasdsda</p><h1 href="#2" class="titulo">Control</h1><p>Temperatura: TEMP01 GRADOS CENTIGRADOS</p><p>Calidad del aire: CAL01 PPM</p><p>Estado ventiladores: VEN01</p><p>Cantidad de humedad: HUM01 %</p><div><canvas id="grafico"></canvas></div><script>const ctx = document.getElementById('grafico');
          
            new Chart(ctx, {
              type: 'scatter',
              data: {
                labels: ['25 seg', '20 seg', '15 seg', '10 seg', '10 seg', '5 seg'],
                datasets: [{
                type: 'line',
                  label: 'Calidad del aire',
                  data: [CAL001, CAL002, CAL003, CAL004, CAL005, CAL006],
                  borderWidth: 1
                },
                {
                  type: 'line',
                  label: 'Temperatura',
                  data: [TEM001, TEM002, TEM003, TEM004, TEM005, TEM006],
                  borderWidth: 1
                },{
                  type: 'line',
                  label: 'Humedad',
                  data: [HUM001, HUM002, HUM003, HUM004, HUM005, HUM006],
                  borderWidth: 1
                },{
                  type: 'bar',
                  label: 'Estado ventiladores',
                  data: [VEN001, VEN002, VEN003, VEN004, VEN005, VEN006],
                  borderWidth: 1
                }]
              },
              options: {
                scales: {
                  y: {
                    beginAtZero: true
                  }
                }
              }
            });</script></body><footer></footer></html>
)=====";