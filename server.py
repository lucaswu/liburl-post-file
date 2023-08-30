from flask import Flask, request

app = Flask(__name__)

@app.route('/upload', methods=['POST'])
def upload_file():
    file = request.files['file']
    file.save(file.filename)
    print(file.filename)
    return 'File uploaded successfully with POST!'

@app.route('/upload', methods=['PUT'])
def upload_file1():
    # file = request.files['file']
    # file.save(file.filename)
    # print(file.filename)
    print(request)
    return 'File uploaded successfully with PUT!'

if __name__ == '__main__':
    app.run(host='0.0.0.0',port=6666,debug=True)