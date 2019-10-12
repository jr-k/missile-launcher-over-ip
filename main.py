from flask import Flask
import subprocess
app = Flask(__name__)


@app.route('/')
def index():
    return "hi"


@app.route('/left')
def doLeft():
    subprocess.run(['/var/www/missilelauncher/usblauncher/ctlmissile', 'left'])
    return "left"

@app.route('/right')
def doRight():
    subprocess.run(['/var/www/missilelauncher/usblauncher/ctlmissile', 'right'])
    return "right"

@app.route('/up')
def doUp():
    subprocess.run(['/var/www/missilelauncher/usblauncher/ctlmissile', 'up'])
    return "up"

@app.route('/down')
def doDown():
    subprocess.run(['/var/www/missilelauncher/usblauncher/ctlmissile', 'down'])
    return "down"

@app.route('/fire')
def doFire():
    subprocess.run(['/var/www/missilelauncher/usblauncher/ctlmissile', 'fire'])
    return "fire"

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=9999)