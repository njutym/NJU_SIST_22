import pymysql

db = pymysql.connect(host='localhost',user='root',password='86161163',port=3306,db='orderdb')

try:
    cursor = db.cursor()

    sql='''
        select c.customerName, c.address, c.telephone
        from Customer c; 
        '''
    cursor.execute(sql) 
    
    def format_row(row):
        return "{:<12}\t{:<12}\t{:<12}".format(*row)

    # 输出表头
    print("名称                    ", "地址            ", "电话        ")

    result = cursor.fetchone()
    while result:
        # 输出数据行
        print(format_row(result))
        result = cursor.fetchone()

except Exception:print("error!")


cursor.close()
db.close()