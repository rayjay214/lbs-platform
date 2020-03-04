// pages/cmd/cmd.js
Page({

  /**
   * 页面的初始数据
   */
  data: {
    urlParams: "https://lite.gmiot.net/wx/command.shtml?access_type=inner&source=app&locale=zh-CN"
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    let url = "";
    for (const key in options) {
      url+= "&" + key + '=' + options[key];
    }
    url = this.data.urlParams + url;
    this.setData({
      urlParams: url
    })
  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})