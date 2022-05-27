# 测试C++获取applicationId和app签名

* 测试发现，获取必须依赖于C++调用Java对象。
* 而bundleId和签名信息，依赖于安卓Context对象
* 因而有可能必须从Java侧发起调用才能获取Context对象（这个目前不清楚有没有其他方法绕过）
