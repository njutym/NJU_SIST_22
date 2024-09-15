import pymysql

db = pymysql.connect(host='localhost',user='root',password='86161163',port=3306,db='orderdb')

try:
    cursor = db.cursor()
    sql = '''
        update
        Product p
        set p.productPrice = 0.5*p.productPrice
        where p.productPrice > 1000;
        '''
    cursor.execute(sql)
    
    sql='''
        select p.productNo, p.productName, p.productPrice
        from Product p; 
        '''
    cursor.execute(sql)
    result=cursor.fetchall()
    for row in result:
        print(row)

except Exception:print("error!")


cursor.close()
db.close()