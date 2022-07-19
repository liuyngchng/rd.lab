# Sequence

## 查看用户序列

```sql
SELECT SEQUENCE_NAME,MIN_VALUE,MAX_VALUE,INCREMENT_BY,LAST_NUMBER FROM USER_SEQUENCES;
```

修改序列

```sql
ALTER SEQUENCE seq_name INCREMENT BY 1 MAXVALUE 10000 CYCLE     -- 到10000后从头开始 NOCACHE ;
```

## create sequence

```sql
CREATE SEQUENCE 序列名

　　[INCREMENT BY n]					// 步长，如果省略，则默认为1，如果出现负值，则代表Oracle序列的值是按照此步长递减的

　　[START WITH n]						// 初始值(即产生的第一个值)，默认为1
　　[{MAXVALUE/ MINVALUE n| NOMAXVALUE}]   // 最大值。选项NOMAXVALUE是默认选项，代表没有最大值定义，这时对于递增Oracle序列，系统能够产生的最大值是10的27次方;对于递减序列，最大值是-1 
　　				// 最小值。选项NOMAXVALUE是默认选项，代表没有最小值定义，这时对于递减序列，系统能够产生的最小值是?10的26次方;对于递增序列，最小值是1
　　[{CYCLE|NOCYCLE}]   		// CYCLE代表循环，NOCYCLE代表不循环。如果循环，则当递增序列达到最大值时，循环到最小值;对于递减序列达到最小值时，循环到最大值。如果不循环，达到限制值后，继续产生新值就会发生错误
　　[{CACHE n| NOCACHE}];   // 存放序列的内存块的大小，默认为20。NOCACHE表示不对序列进行内存缓冲。对序列进行内存缓冲，可以改善序列的性能
```

example

```sql
 create sequence seq_name increment by 1 start with 1;
```

使用

```sql
insert into t1 values(seq_name.nextval, 1, 1);
```

# record num

```sql
SELECT 
   table_name, count_rows(table_name) nrows 
FROM 
    user_tables 
ORDER BY 
    nrows DESC;
```

