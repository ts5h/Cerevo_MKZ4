// HTML string for controlling MKZ4 from smartphone
String form = R"#(
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1, minimum-scale=1, maximum-scale=1">
  <title>MKZ4 Controller</title>
  <style type="text/css">
    * {
      box-sizing: border-box;
      margin: 0;
      padding: 0;
    }

    html, body {
      width: 100%;
      height: 100%
    }

    body {
      display: flex;
      justify-content: center;
      align-items: center;
      width: 100%;
      height: 100%;
      background-color: #0097c1;
    }

    #value {
      position: fixed;
      width: 100%;
      top: 30px;
      text-align: center;
      color: white;
      font-family: sans-serif;
      font-size: 1rem;
      font-weight: normal;
      text-transform: capitalize;
    }

    #tif {
      display: none;
    }

    #controller__wrapper {
      position: relative;
      width: 282px;
      height: 282px;
      border: 1px solid rgba(255, 255, 255, 0.5);
    }

    #controller {
      position: absolute;
      width: 80px;
      height: 80px;
      left: calc(50% - 25px);
      top: calc(50% - 25px);
      background: rgba(255, 255, 255, 0.2);
      border: 1px solid white;
      border-radius: 50%;
    }
  </style>
</head>

<body>
  <div id="value">connected</div>
  <form id="form" action="" target="tif">
    <iframe id="tif" name="tif" src="javascript: false;"></iframe>
  </form>
  <div id="controller__wrapper">
    <div id="controller"></div>
  </div>

  <script type="text/javascript">
    var espPort = 'http://192.168.4.1:8080';
    var command = '/stop';
    
    var value = document.getElementById('value');
    var form = document.getElementById('form');
    var wrapper = document.getElementById('controller__wrapper');
    var controller = document.getElementById('controller');

    var touchFlag = false;
    var controllerOffset = controller.clientWidth / 2;
    var wrapperOffsetX = wrapper.offsetLeft;
    var wrapperOffsetY = wrapper.offsetTop;
    var startX = 100;
    var startY = 100;
    var maxX = 200;
    var maxY = 200;

    var showValue = function (state) {
      value.innerHTML = state;
    };

    var moveHomePosition = function () {
      controller.style.left = startX;
      controller.style.top = startY;
    };

    wrapper.ontouchstart = function (e) {
      e.preventDefault();
      touchFlag = true;
    };

    wrapper.ontouchmove = function (e) {
      if (!touchFlag) return;
      e.preventDefault();

      var touch = e.touches[0];
      var controllerLeft = touch.clientX - wrapperOffsetX - controllerOffset;
      var controllerTop = touch.clientY - wrapperOffsetY - controllerOffset;
      var x = 0;
      var y = 0;

      if (controllerLeft <= 0) controllerLeft = 0;
      if (controllerLeft >= maxX) controllerLeft = maxX;
      if (controllerTop <= 0) controllerTop = 0;
      if (controllerTop >= maxY) controllerTop = maxY;

      controller.style.left = controllerLeft;
      controller.style.top = controllerTop;
      x = controllerLeft - startX;
      y = (controllerTop - startY) * -1;

      form.action = espPort + '/move?x=' + x + '&y=' + y;
      form.submit();
    };

    wrapper.ontouchend = function (e) {
      e.preventDefault();
      touchFlag = false;
      form.action = espPort + '/stop';
      form.submit();
      setTimeout(moveHomePosition, 50);
    }

    window.onload = function () {
      moveHomePosition();
    };
  </script>
</body>
</html>
)#";