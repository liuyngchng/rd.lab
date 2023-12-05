

# spring boot 配置文件中设置DB密文密码

在 resources/ 下新建文件夹 META-INF， 下面新建文件  spring.factories

vi spring.factories

```sh
org.springframework.boot.autoconfigure.EnableAutoConfiguration=com.a.b.config.DbCfg
```

其中 com.a.b.config.DbCfg 就是具体的配置文件

vi DbCfg.java

```java
package com.a.b.config;

import com.a.b.utl.RSAUtl;
import com.zaxxer.hikari.HikariDataSource;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.boot.autoconfigure.jdbc.DataSourceProperties;
import org.springframework.boot.context.properties.EnableConfigurationProperties;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import javax.annotation.Resource;
import javax.sql.DataSource;

/**
 * customized DataSource
 */
@Configuration
@EnableConfigurationProperties(DataSourceProperties.class)
public class DbCfg {

    private static final Logger LOGGER = LogManager.getLogger();

    @Resource
    private DataSourceProperties prop;

    @Bean(name = "dataSource")
    public DataSource getDS() throws Exception {
        LOGGER.info("customized_db");
        final HikariDataSource ds = new HikariDataSource();
        ds.setJdbcUrl(this.prop.getUrl());
        ds.setUsername(this.prop.getUsername());
        ds.setDriverClassName(this.prop.getDriverClassName());
        try {
            // 解密密码
            ds.setPassword(RSAUtl.dec(this.prop.getPassword()));
        } catch (Exception ex) {
            LOGGER.error("err_dec_psw", ex);
            throw ex;
        }
        ds.setDriverClassName(this.prop.getDriverClassName());
        return ds;
    }
}
```

