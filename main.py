from flask import Flask
import subprocess
app = Flask(__name__)


@app.route('/')
def index():
    return "hi"


@app.route('/left')
def doLeft():
    subprocess.run(['./usblauncher/ctlmissile', 'left'])
    return "left"

@app.route('/right')
def doRight():
    subprocess.run(['./usblauncher/ctlmissile', 'right'])
    return "right"

@app.route('/up')
def doUp():
    subprocess.run(['./usblauncher/ctlmissile', 'up'])
    return "up"

@app.route('/down')
def doDown():
    subprocess.run(['./usblauncher/ctlmissile', 'down'])
    return "down"

@app.route('/fire')
def doFire():
    subprocess.run(['./usblauncher/ctlmissile', 'fire'])
    return "fire"

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=9999)