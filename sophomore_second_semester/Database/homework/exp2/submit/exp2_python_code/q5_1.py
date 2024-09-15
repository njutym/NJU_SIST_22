import pymysql

db = pymysql.connect(host='localhost',user='root',password='86161163',port=3306,db='orderdb')

try:
    cursor = db.cursor()
    sql = '''
        update
        Employee e
        set e.salary = 200 + e.salary
        where e.department = '业务科'
        '''
    cursor.execute(sql)
    
    sql='''
        select e.employeeNo, e.employeeName, e.salary
        from Employee e; 
        '''
    cursor.execute(sql)
    result=cursor.fetchall()
    for row in result:
        print(row)

except Exception:print("error!")


cursor.close()
db.close()