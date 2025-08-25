import pymysql

db = pymysql.connect(host='localhost',user='root',password='86161163',port=3306,db='orderdb')

try:
    cursor = db.cursor()
    sql = '''
        insert Customer values('C20080002','泰康股份有限公司','010-5422685', '天津市','220501');
        '''
    cursor.execute(sql)
    
    sql='''
        select * from Customer;
        '''
    cursor.execute(sql)
    result=cursor.fetchall()
    for row in result:
        print(row)

except Exception:print("error!")


cursor.close()
db.close()