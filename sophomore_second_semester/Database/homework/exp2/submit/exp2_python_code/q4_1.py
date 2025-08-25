import pymysql

db = pymysql.connect(host='localhost',user='root',password='86161163',port=3306,db='orderdb')

try:
    cursor = db.cursor()
    sql = '''
        select employeeNo, employeeName, salary 
        from Employee 
        order by salary DESC 
        limit 20
        '''
    cursor.execute(sql)
    result = cursor.fetchall()
    for row in result:
        print(row)
except Exception:print("error!")


cursor.close()
db.close()