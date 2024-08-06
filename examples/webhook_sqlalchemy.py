#!/usr/bin/env python
# coding: utf-8

# In[ ]:


from flask import Flask, request, json
from flask_sqlalchemy import SQLAlchemy

app = Flask(__name__)
app.config["SQLALCHEMY_DATABASE_URI"] = "mysql+pymysql://root:chenzhenglab@localhost:3306/mqtt"

db = SQLAlchemy()
db.init_app(app)

class message(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    request_string = db.Column(db.String(200), unique=True, nullable=True)

with app.app_context():
    db.create_all()
    
@app.route('/')
def app_root():
    return 'the web server is on'

@app.route('/webhook', methods = ['POST'])
def app_webhook():
    body = str(request.get_data())
    msg = message(request_string = body)
    db.session.add(msg)
    db.session.commit()
    return 'success',200

if __name__ == '__main__':
    app.run(host='10.24.165.222',port=5006)


# In[ ]:




