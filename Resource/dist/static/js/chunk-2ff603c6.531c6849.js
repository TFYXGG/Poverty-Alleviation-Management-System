(window["webpackJsonp"]=window["webpackJsonp"]||[]).push([["chunk-2ff603c6"],{1:function(e,t){},2:function(e,t){},3:function(e,t){},4:function(e,t){},9406:function(e,t,i){"use strict";i.r(t);var n=function(){var e=this,t=e.$createElement,i=e._self._c||t;return i("div",{staticClass:"app-container"},[i("el-table",{directives:[{name:"loading",rawName:"v-loading",value:e.listLoading,expression:"listLoading"}],attrs:{data:e.listA,"element-loading-text":"Loading",border:"",fit:"","highlight-current-row":""}},[i("el-table-column",{attrs:{align:"center",label:"ID",width:"95"},scopedSlots:e._u([{key:"default",fn:function(t){return[e._v(e._s(t.$index))]}}])}),e._v(" "),i("el-table-column",{attrs:{label:"景点名称",width:"310"},scopedSlots:e._u([{key:"default",fn:function(t){return[e._v(e._s(t.row.name))]}}])}),e._v(" "),i("el-table-column",{attrs:{label:"景点票价",width:"110",align:"center"},scopedSlots:e._u([{key:"default",fn:function(t){return[i("span",[e._v(e._s(t.row.fare))])]}}])}),e._v(" "),i("el-table-column",{attrs:{label:"地址",width:"110",align:"center"},scopedSlots:e._u([{key:"default",fn:function(t){return[e._v(e._s(t.row.address))]}}])}),e._v(" "),i("el-table-column",{attrs:{align:"center",prop:"created_at",label:"删除景点",width:"120"},scopedSlots:e._u([{key:"default",fn:function(t){return[i("el-button",{on:{click:function(i){return e.deleteData(t.row)}}},[e._v("确认删除")])]}}])}),e._v(" "),i("el-table-column",{attrs:{align:"center",prop:"created_at",label:"编辑景点",width:"120"},scopedSlots:e._u([{key:"default",fn:function(t){return[i("el-button",{on:{click:function(i){return e.editViewData(t.row)}}},[e._v("编辑")])]}}])})],1),e._v(" "),i("div",{directives:[{name:"show",rawName:"v-show",value:e.showRegisterBox,expression:"showRegisterBox"}],staticClass:"modelBoxWrapper"},[i("div",{staticClass:"registerBox"},[i("div",{staticStyle:{position:"absolute",top:"5px",right:"5px",cursor:"pointer"},on:{click:e.closeRegisterBox}},[e._v("X")]),e._v(" "),i("div",[i("el-form",{ref:"form",attrs:{"label-width":"120px"}},[i("el-form-item",{attrs:{label:"景点名称"}},[i("el-input",{model:{value:e.editViewInfo.name,callback:function(t){e.$set(e.editViewInfo,"name",t)},expression:"editViewInfo.name"}})],1),e._v(" "),i("el-form-item",{attrs:{label:"景点票价"}},[i("el-input",{model:{value:e.editViewInfo.fare,callback:function(t){e.$set(e.editViewInfo,"fare",t)},expression:"editViewInfo.fare"}})],1),e._v(" "),i("el-form-item",{attrs:{label:"景点地址"}},[i("el-input",{model:{value:e.editViewInfo.address,callback:function(t){e.$set(e.editViewInfo,"address",t)},expression:"editViewInfo.address"}})],1),e._v(" "),i("el-form-item",[i("el-button",{on:{click:e.submitForm}},[e._v("提交")])],1)],1)],1)])])],1)},o=[],s=(i("3a23"),i("ad8f")),a=i("7f43"),l=i.n(a),r=(i("7b04"),{filters:{statusFilter:function(e){var t={published:"success",draft:"gray",deleted:"danger"};return t[e]}},data:function(){return{editViewInfo:{name:"",address:"",fare:"",id:""},list:null,listLoading:!1,listA:[],showRegisterBox:!1,showLoginBox:!0,showModelbox:!1}},created:function(){this.getViewListData()},methods:{closeRegisterBox:function(){this.showRegisterBox=!1},openRegisterBox:function(){this.showLoginBox&&this.closeLoginBox(),this.showRegisterBox=!0},closeModel:function(){this.showModelbox=!1},getViewListData:function(){var e=this;l.a.get("http://127.0.0.1:3000/getviewlistinfo").then(function(t){e.listA=t.data.x})},deleteData:function(e){var t=this;l.a.post("http://127.0.0.1:3000/deleteviewinfo",{id:e.id}).then(function(e){t.$message({message:"删除成功！",type:"success"}),t.getViewListData()}).catch(function(e){t.$message({message:"删除失败！",type:"warning"}),console.log(e)})},editViewData:function(e){this.editViewInfo.name=e.name,this.editViewInfo.address=e.address,this.editViewInfo.fare=e.fare,this.editViewInfo.id=e.id,this.showRegisterBox=!0},submitForm:function(){var e=this;l.a.post("http://127.0.0.1:3000/updateviewinfo",this.editViewInfo).then(function(t){e.getViewListData(),e.showRegisterBox=!1,e.$message({message:"编辑成功！",type:"success"})})},fetchData:function(){var e=this;this.listLoading=!0,Object(s["a"])().then(function(t){e.list=t.data.items,e.listLoading=!1})}}}),c=r,d=(i("b8d5"),i("17cc")),u=Object(d["a"])(c,n,o,!1,null,"22a3a9de",null);t["default"]=u.exports},ad8f:function(e,t,i){"use strict";i.d(t,"a",function(){return o});var n=i("b775");function o(e){return Object(n["a"])({url:"/table/list",method:"get",params:e})}},b8d5:function(e,t,i){"use strict";var n=i("fee6"),o=i.n(n);o.a},fee6:function(e,t,i){}}]);