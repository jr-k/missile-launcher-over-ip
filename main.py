from flask import Flask
from flask import request
import subprocess
app = Flask(__name__)


@app.route('/')
def index():
    return "hi"


@app.route('/left')
def doLeft():
    lenx = request.args.get('length', 1)
    subprocess.run(['./usblauncher/ctlmissile', 'left', lenx])
    return "left"

@app.route('/right')
def doRight():
    lenx = request.args.get('length', 1)
    subprocess.run(['./usblauncher/ctlmissile', 'right', lenx])
    return "right"

@app.route('/up')
def doUp():
    lenx = request.args.get('length', 1)
    subprocess.run(['./usblauncher/ctlmissile', 'up', lenx])
    return "up"

@app.route('/down')
def doDown():
    lenx = request.args.get('length', 1)
    subprocess.run(['./usblauncher/ctlmissile', 'down', lenx])
    return "down"

@app.route('/fire')
def doFire():
    lenx = request.args.get('length', 1)
    subprocess.run(['./usblauncher/ctlmissile', 'fire', lenx])
    return "fire"

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=9999)