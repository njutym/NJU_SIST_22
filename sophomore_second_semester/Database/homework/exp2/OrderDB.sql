drop database if exists `OrderDB`;
create database OrderDB;
use OrderDB;

/*人事表*/
CREATE TABLE Employee(
  employeeNo   char(8)        not null,             /*员工编号*/
  employeeName varchar(10)    not null,             /*员工姓名*/
  gender          char(1)        not null,             /*员工性别*/
  birthday     datetime       null,                 /*员工生日*/
  address      varchar(50)    null,                 /*员工住址*/
  telephone    varchar(20)    null,                 /*员工电话*/
  hireDate     datetime       not null,             /*雇佣日期*/
  department   varchar(30)    not null,             /*所属部门*/
  headShip     varchar(10)    not null,             /*职务*/
  salary       numeric(8,2)   not null,             /*薪水*/
  constraint EmployeePK primary key (employeeNo)
);

/*人事表（employee）数据：*/
insert Employee values('E2005001','喻自强','M','19650415','南京市青海路18号','13817605008', '19900206','财务科','科长',5800);
insert Employee values('E2005002','张小梅','F','19731101','上海市北京路8号','13607405016', '19910328','业务科','职员',2400);
insert Employee values('E2005003','张小娟','F','19730306','上海市南京路66号','13707305025', '19920328','业务科','职员',2600);
insert Employee values('E2005004','张露',  'F','19670105','南昌市八一大道130号','15907205134', '19900328','业务科','科长',4100);
insert Employee values('E2005005','张小东','M','19730903','南昌市阳明路99号','15607105243', '19920328','业务科','职员',1800);
insert Employee values('E2006001','陈辉',  'M','19651101','南昌市青山路100号','13607705352', '19900328','办公室','主任',4000);
insert Employee values('E2006002','韩梅',  'F','19731211','上海市浦东大道6号','13807805461', '19901128','业务科','职员',2600);
insert Employee values('E2006003','刘风',  'F','19730521','江西财经大学5栋1-101室','15907805578', '19910228','业务科','职员',2500);
insert Employee values('E2007001','吴浮萍','M','19730912','南昌高新开发区12号', null,'19900628','业务科','职员',2500);
insert Employee values('E2007002','高代鹏','M','19730102','南昌高新开发区56号', null,  '19911128','办公室','文员',2000);
insert Employee values('E2008001','陈诗杰','M','19680106','江西财经大学12栋3-304室',null,  '19901206','财务科','出纳',3200);
insert Employee values('E2008002','张良',  'M','19720216','上海市福州路135号',null,  '19920228','业务科','职员',2700);
insert Employee values('E2008003','黄梅莹','F','19720515','上海市九江路88号',null,  '19910228','业务科','职员',3100);
insert Employee values('E2008004','李虹冰','F','19721013','南昌市中山路1号',null,  '19900528','业务科','职员',3400);
insert Employee values('E2008005','张小梅','F','19701106','深圳市阳关大道10号',null,  '19901118','财务科','会计',5000);

/*客户表*/
create table  Customer(
  customerNo    char(9)      not null primary key,/*客户号*/
  customerName  varchar(40)  not null,            /*客户名称*/
  telephone     varchar(20)  not null,            /*客户电话*/
  address       char(40)     not null,            /*客户住址*/
  zip           char(6)      null                 /*邮政编码*/
);

/*客户表（customer）数据：*/ 
insert Customer values('C20050001','统一股份有限公司',  '022-3566021',  '天津市', '220012');
insert Customer values('C20050002','兴隆股份有限公司',  '022-3562452',  '天津市', '220301');
insert Customer values('C20050003','上海生物研究室',    '010-2121000',  '北京市', '108001');
insert Customer values('C20050004','五一商厦',          '021-4532187',  '上海市', '210100');
insert Customer values('C20060001','大地商城',          '010-1165152',  '北京市', '100803');
insert Customer values('C20060002','联合股份有限公司',  '021-4568451',  '上海市', '210100');
insert Customer values('C20070001','南昌市电脑研制中心','0791-4412152', '南昌市', '330046');
insert Customer values('C20070002','世界技术开发公司',  '021-4564512',  '上海市', '210230');
insert Customer values('C20070003','万事达股份有限公司','022-4533141',  '天津市', '220400');
insert Customer values('C20080001','红度股份有限公司',  '010-5421585',  '北京市', '100800');


create table Product(
  productNo     char(9)       not null primary key, /*商品编号*/
  productName   varchar(40)   not null,             /*商品名称*/
  productClass  varchar(20)   not null,             /*商品类别*/
  productPrice  numeric(7,2)  not null           /*商品定价*/  
);

-- delimiter <>
-- drop trigger if exists q3_1 <>
-- create trigger q3_1 
-- before insert on Product
-- 	for each row
-- begin
-- 	if new.productPrice > 1000 
-- 		then set new.productPrice = 1000;
-- 	end if ;
-- end 
-- <>
-- delimiter ;


insert Product values('P20050001','32M DRAM',             '内存',80.70);
insert Product values('P20050002','17寸显示器',           '显示器',700.00);
insert Product values('P20050003','120GB硬盘',            '存储器',300.00);
insert Product values('P20050004','3.5寸软驱',            '设备',35.00);
insert Product values('P20050005','键盘',                 '设备',100.60);
insert Product values('P20060001','VGA显示卡',            '显示器',1200.60);
insert Product values('P20060002','网卡',                 '设备',66.00);
insert Product values('P20060003','Pentium100CPU',        '处理器',200.00);
insert Product values('P20070001','1G DDR',               '内存',256.00);
insert Product values('P20070002','52倍速光驱',           '设备',200.00);
insert Product values('P20070003','计算机字典',           '图书',100.00);
insert Product values('P20070004','9600bits/s调制解调',   '设备',320.00);
insert Product values('P20080001','Pentium主板',          '主板',890.00);
insert Product values('P20080002','硕泰克SL—K8AN-RL主板','主板',1100.00);
insert Product values('P20080003','龙基777FT纯平显示器',  '显示器',900.00);

/*订单主表*/
create table OrderMaster(
  orderNo     char(12)     not null primary key,/*定单编号*/
  customerNo  char(9)      not null,            /*客户号*/
  employeeNo     char(8)      not null,            /*业务员编号*/
  orderDate   datetime     not null,            /*定货日期*/
  orderSum    numeric(9,2) not null,            /*定单金额*/
  invoiceNo   char(10)     not null,            /*发票号码*/
  constraint OrderMasterFK1 foreign key(customerNo) references Customer(customerNo), 
  constraint OrderMasterFK2 foreign key(employeeNo) references Employee(employeeNo)   
);

-- delimiter <>
-- drop trigger if exists q3_2 <>
-- create trigger q3_2
-- 	after insert on OrderMaster
-- 	for each row
-- begin
-- 	declare hire_year int;
--     select YEAR(hireDate) into hire_year from Employee where employeeNo = new.employeeNo;
-- 	if hire_year >= '1992' 
-- 		then
-- 			update Employee set salary = salary * 1.05 where employeeNo = new.employeeNo;
-- 		else
-- 			update Employee set salary = salary * 1.08 where employeeNo = new.employeeNo;
--     end if;
-- end 
-- <>
-- delimiter ;


/*订单主表（sales）数据：*/
insert OrderMaster values('200801090001','C20050001','E2005002','20080109',0.00,'I000000001'); 
insert OrderMaster values('200801090002','C20050004','E2005003','20080109',0.00,'I000000002');
insert OrderMaster values('200801090003','C20080001','E2005002','20080109',0.00,'I000000003');
insert OrderMaster values('200802190001','C20050001','E2005003','20080219',0.00,'I000000004');
insert OrderMaster values('200802190002','C20070002','E2008002','20080219',0.00,'I000000005');
insert OrderMaster values('200803010001','C20070002','E2008001','20080301',0.00,'I000000006');
insert OrderMaster values('200803020001','C20050004','E2008003','20080302',0.00,'I000000007');
insert OrderMaster values('200803090001','C20070003','E2008004','20080309',0.00,'I000000008');
insert OrderMaster values('200805090001','C20060002','E2008002','20080509',0.00,'I000000009');
insert OrderMaster values('200806120001','C20050001','E2005002','20080612',0.00,'I000000010');

/*订单明细表*/
create table OrderDetail(
  orderNo   char(12)      not null,            /*定单编号*/
  productNo char(9)       not null,            /*产品编号*/
  quantity  int           not null,            /*销售数量*/
  price     numeric(7,2)  not null,            /*订货单价*/
  constraint OrderDetailPK primary key clustered(orderNo,productNo),
  constraint OrderDetailFK1 foreign key(orderNo) references OrderMaster(orderNo), 
  constraint OrderDetailFK2 foreign key(productNo) references Product(productNo)   
);

/*订单明细表（OrderDetail）数据：*/
insert OrderDetail values('200801090001','P20050001',5 , 500.00);
insert OrderDetail values('200801090001','P20050002',3 , 500.00);
insert OrderDetail values('200801090001','P20050003',2 , 300.00);
insert OrderDetail values('200806120001','P20050004',2 , 600.00);
insert OrderDetail values('200806120001','P20050005',3 , 600.00);
insert OrderDetail values('200806120001','P20060001',1 , 300.00);
insert OrderDetail values('200806120001','P20060002',2 , 280.00);
insert OrderDetail values('200802190001','P20060003',4 , 270.00);
insert OrderDetail values('200802190001','P20070001',2 , 158.00);
insert OrderDetail values('200802190001','P20070002',5 , 250.00);
insert OrderDetail values('200802190001','P20070003',3 , 350.00);
insert OrderDetail values('200802190001','P20070004',2 , 330.00);
insert OrderDetail values('200802190001','P20080001',2 , 160.00);
insert OrderDetail values('200802190001','P20080002',3 , 260.00);
insert OrderDetail values('200802190001','P20080003',1 , 330.00);
insert OrderDetail values('200801090002','P20080001',5 , 280.00);
insert OrderDetail values('200801090002','P20080002',4 , 270.00);
insert OrderDetail values('200801090002','P20080003',2 , 158.00);
insert OrderDetail values('200801090002','P20060002',5 , 250.00);
insert OrderDetail values('200801090003','P20060001',3 , 350.00);
insert OrderDetail values('200801090003','P20050001',5 , 130.00);
insert OrderDetail values('200802190002','P20050003',2 , 160.00);
insert OrderDetail values('200802190002','P20050005',3 , 150.00);
insert OrderDetail values('200803010001','P20070001',4 , 150.00);
insert OrderDetail values('200803010001','P20050001',8 , 150.00);

insert OrderDetail values('200803020001','P20050001',2 , 100.00);
insert OrderDetail values('200803020001','P20070003',3 , 200.00);
insert OrderDetail values('200803020001','P20050002',1 , 200.00);
insert OrderDetail values('200803090001','P20070002',5 , 380.00);
insert OrderDetail values('200803090001','P20050003',4 , 200.00);
insert OrderDetail values('200803090001','P20070001',2 , 158.00);
insert OrderDetail values('200803090001','P20050004',5 , 250.00);
insert OrderDetail values('200803090001','P20070004',3 , 350.00);
insert OrderDetail values('200805090001','P20070003',5 , 300.00);
insert OrderDetail values('200805090001','P20070002',2 , 600.00);
insert OrderDetail values('200802190002','P20070001',3 , 500.00);
insert OrderDetail values('200805090001','P20070001',4 , 500.00);
insert OrderDetail values('200805090001','P20060003',8 , 300.00);



