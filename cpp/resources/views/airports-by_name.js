function (doc, meta) {
	
  if (typeof doc.type !== "undefined" && doc.type == "airport")
  {
     emit(meta.id, doc.airportname);  
  }
}
