const char PAGINA_PRINCIPAL[] = 
R"=====( 
  <!DOCTYPE html><html><script src="https://cdn.jsdelivr.net/npm/chart.js"></script><style>header{background-color:#08a1c7;position:relative;height:50px}header h1{font-family:monospace;margin:0;position:absolute;top:50%;left:50%;transform:translate(-50%,-50%)}.menu ul{display:flex;padding:0;margin:0;list-style:none}.menu a{display:block;padding:1em;background-color:#08a1c7;text-decoration:none;color:#191c26}.menu a:hover{background-color:#036988}.menu ul li ul{display:none;position:absolute;top:100%;left:0;background-color:#08a1c7;padding:0;margin:0;list-style:none;width:100px}.menu ul li ul li a{padding:1em;background-color:#08a1c7;width:100%}.menu ul li a:hover+ul,.menu ul li ul:hover{display:block}a,p,label{font-size:large;font-family:monospace}h1{font-family:monospace}.titulo{margin-left:10px}canvas{max-width:700px;max-height:300px}body{max-width:50%;margin-left:25%}</style>
<header><nav class="menu"><ul><li><a href="#">Menu</a><ul><li><a href="#1">Proyecto de Monitoreo de Gas y Condiciones Ambientales con ESP32</a></li><li><a href="#2">Control</a></li><li><a href="https://docs.google.com/document/d/1LpznAeZUYXJSYKVT7ExkfE8q8Cioc0Tal1mJyOyHRgQ/edit?usp=drivesdk">Carpeta tecnica</a></li></ul></li></ul></nav><h1>ESP CALIDAD DEL AIRE</h1></header><body><h1 href="#1" class="titulo">Proyecto de Monitoreo de Gas y Condiciones Ambientales con ESP32</h1><p>Este proyecto utiliza una plataforma ESP32 para la monitorizacion avanzada de la calidad del aire y las condiciones ambientales mediante la integracion de sensores especificos. La iniciativa tiene como objetivo proporcionar un sistema efectivo de alerta en caso de niveles elevados de gases peligrosos, ademas de medir temperatura y humedad, datos fundamentales para diversas aplicaciones domesticas.</p><h1 href="#2" class="titulo">Control</h1><p>Temperatura: TEMP01 GRADOS CENTIGRADOS</p><p>Calidad del aire: CAL01 %</p><p>Estado ventiladores: VEN01</p>

<button onclick="toggleVentilador()">Encender/Apagar Ventilador</button>

<script>
  function toggleVentilador() {
    fetch("/toggleVentilador")
      .then(response => response.json())
      .then(data => {
        // Actualiza el estado del botón según el estado del ventilador
        if (data.ventilador == 1) {
          alert("Ventilador Encendido");
        } else {
          alert("Ventilador Apagado");
        }
      });
  }
</script>

<!-- Formulario para controlar el ventilador por temporizador -->
<form id="temporizadorForm">
  <label for="tiempo">Tiempo en segundos:</label>
  <input type="number" id="tiempo" name="tiempo" min="1" max="3600" required>
  <button type="submit">Configurar Temporizador</button>
</form>

<p>Cantidad de humedad: HUM01 %</p>

<script>
  document.getElementById('temporizadorForm').addEventListener('submit', function(event) {
    event.preventDefault();  // Evita el envío del formulario
    var tiempoSegundos = document.getElementById('tiempo').value;  // Obtiene el tiempo ingresado
    fetch(`/startTimer?tiempo=${tiempoSegundos}`)  // Envia el tiempo al servidor
      .then(response => response.json())
      .then(data => {
        alert("Temporizador configurado para " + tiempoSegundos + " segundos.");
      });
  });
</script>


<div><canvas id="grafico"></canvas></div>
<h1 href="#1" class="titulo">Caracteristicas y Componentes</h1>
<p>La estructura principal del sistema esta construida sobre el microcontrolador ESP32, conocido por su capacidad de conectividad Wi-Fi y Bluetooth, su versatilidad y su potencia de procesamiento. Este dispositivo ofrece una plataforma robusta y adaptable para aplicaciones de Internet de las Cosas (IoT), permitiendo el monitoreo remoto y la recoleccion de datos en tiempo real.</p>
<p>El sensor de gas MQ-9 es una pieza clave para la deteccion de monoxido de carbono (CO) y gases combustibles como el metano. Este dispositivo mide concentraciones de gas en el ambiente y permite alertar en caso de que los niveles sean elevados, activando de esta forma un sistema de alerta que es critico para la prevencion de incendios y situaciones peligrosas.</p>
<p>Complementando el sistema de monitoreo, el sensor DHT11 se encarga de captar la temperatura y humedad del entorno. Estos datos son esenciales no solo para entender las condiciones ambientales sino tambien para detectar posibles situaciones que podrian agravar una fuga de gas.</p>
<script>const ctx = document.getElementById('grafico');
          
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
                animation: false,
                scales: {
                  y: {
                    beginAtZero: true
                  }
                }
              }
            });</script>
            
            <script>
  setInterval(() => {
    location.reload();
  }, 5000); // Recarga la página cada 5 segundos
</script>
</body><footer></footer></html>
)=====";
