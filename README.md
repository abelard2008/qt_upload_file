## 参考
[Qt利用post上传图像到SM.MS图床](https://www.codenong.com/js22f2025adf34/)
## 修改
原文章使用的是早期的api，不需要鉴权，现在需要注册并获得自己的token，在代码中：
```
 QUrl url("https://sm.ms/api/v2/upload"); //新的接口
 request.setRawHeader("Authorization","ApyfgSlDmRMVrz5gi4VEMuDzq86vDZpwxxox");
```
## 


