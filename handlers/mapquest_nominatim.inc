<?php

/**
 * @file
 * Plugin to provide a MapQuest Nominatim geocoder.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("MapQuest Nominatim"),
  'description' => t('Geocodes via MapQuest Nominatim'),
  'callback' => 'geocoder_mapquest_nominatim',
  'field_types' => array('text', 'text_long', 'addressfield', 'location', 'text_with_summary', 'computed', 'taxonomy_term_reference'),
  'field_callback' => 'geocoder_mapquest_nominatim_field',
  'terms_of_service' => 'http://developer.mapquest.com/web/info/terms-of-use',
);

/**
 * Process Markup
 */
function geocoder_mapquest_nominatim($address, $options = array()) {
  $geocoder_settings = variable_get("geocoder_settings", array());
//$api_url = "http://open.mapquestapi.com/nominatim/v1/search";
// 403 The AppKey submitted with this request is invalid.
//$api_url = "http://www.mapquestapi.com/geocoding/v1/address?key=qRGHUutwAEK6mOKSXyToVrnmR4S5Gvr2";
  $api_url = "http://open.mapquestapi.com/nominatim/v1/search.php?key=qRGHUutwAEK6mOKSXyToVrnmR4S5Gvr2";
//$api_url = "https://nominatim.openstreetmap.org/search.php";
  $params = array(
//  'q' => str_replace(' ', '+', $address),
    'format' => 'json',
    'addressdetails' => 0,
    'limit' => 1,
    'osm_type' => 'N',
  );
//$request = drupal_http_request($api_url . '?' . drupal_http_build_query($params));
//$request = drupal_http_request($api_url . '?q=' . urlencode($address) . '&' . drupal_http_build_query($params));
//$request = drupal_http_request($api_url . '&location=' . urlencode($address) . '&addressdetails=0&limit=1&osm_type=N&format=json');
  $request = drupal_http_request($api_url . '&format=json&osm_type=N&q='.urlencode($address));
//$request = drupal_http_request($api_url . '?format=json&countrycodes=fr&q='.urlencode($address));
dpm($request);
  $data = json_decode($request->data);
/*
 * mapquest new
  $rawdata = json_decode($request->data);
  $lat = $rawdata->results[0]->locations[0]->latLng->lat;
  $lon = $rawdata->results[0]->locations[0]->latLng->lng; // lng, not lon
  $loc = new stdclass;
  $loc->lat = $lat;
  $loc->lon = $lon;
  $data = array($loc);
*/
dpm($data);
  return _geocoder_mapquest_nominatim_geometry($data);
}
/*
My Application’s Keys
Consumer Key 	qRGHUutwAEK6mOKSXyToVrnmR4S5Gvr2
Consumer Secret 	vguNfCYnZwdtWJhb
Key Issued 	Wed, 11/18/2015 - 05:49
Key Expires 	Never

[{
  "place_id":"121033594",
  "licence":"Data \u00a9 OpenStreetMap contributors, ODbL 1.0. http:\/\/www.openstreetmap.org\/copyright",
  "osm_type":"way",
  "osm_id":"196097064",
  "boundingbox":["45.5342072","45.5471073","6.2125133","6.2166785"],
  "lat":"45.5406069",
  "lon":"6.2137878",
  "display_name":"Route de l'Arclusaz, Chamoux-sur-Gelon, Chamb\u00e9ry, Savoie, Rh\u00f4ne-Alpes, France m\u00e9tropolitaine, 73390, France",
  "class":"highway",
  "type":"unclassified",
  "importance":0.8
}]

http://www.mapquestapi.com/geocoding/v1/address?key=YOUR_KEY_HERE&location=Lancaster,PA&callback=renderGeocode

{
  "info":{
    "statuscode":0,
    "copyright":{"text":"\u00A9 2015 MapQuest, Inc.","imageUrl":"http://api.mqcdn.com/res/mqlogo.gif","imageAltText":"\u00A9 2015 MapQuest, Inc."},
    "messages":[]
  },
  "options":{
    "maxResults":-1,"thumbMaps":true,"ignoreLatLngInput":false
  },
  "results":[{
    "providedLocation":{
      "location":"416 Route de l'Arclusaz 73390 Chamoux-sur-Gelon"
    },
    "locations":[{
      "street":"Route de l'Arclusaz",
      "adminArea6":"",
      "adminArea6Type":"Neighborhood",
      "adminArea5":"Chamoux-sur-Gelon",
      "adminArea5Type":"City",
      "adminArea4":"Chamb\u00E9ry",
      "adminArea4Type":"County",
      "adminArea3":"Rh\u00F4ne-Alpes",
      "adminArea3Type":"State",
      "adminArea1":"FR","adminArea1Type":"Country",
      "postalCode":"73390",
      "geocodeQualityCode":"B1AAA",
      "geocodeQuality":"STREET",
      "dragPoint":false,
      "sideOfStreet":"N",
      "linkId":"0",
      "unknownInput":"",
      "type":"s",
      "latLng":{
        "lat":45.540607,
        "lng":6.213788
      },
      "displayLatLng":{"lat":45.540607,"lng":6.213788},
      "mapUrl":"http://www.mapquestapi.com/staticmap/v4/getmap?key=qRGHUutwAEK6mOKSXyToVrnmR4S5Gvr2&type=map&size=225,160&pois=purple-1,45.5406069,6.2137878,0,0,|&center=45.5406069,6.2137878&zoom=15&rand=-434429410"
    }]
  }
  ]}


*/
/*
http://nominatim.openstreetmap.org/search?q=181 RUE JEAN JAURÈS 80016 AMIENS&json_callback=_l_osmgeocoder_0&format=json
http://nominatim.openstreetmap.org/search?q=416 Route de l'Arclusaz Chamoux-sur-Gelon&json_callback=_l_osmgeocoder_6&format=json
http://nominatim.openstreetmap.org/search?q=416 Route de l'Arclusaz Chamoux-sur-Gelon&osm_type=N&format=json

https://nominatim.openstreetmap.org/search.php?q=416+route+de+l%27arclusaz+chamoux-sur-gelon&format=json

[{"place_id":"101214817","licence":"Data © OpenStreetMap contributors, ODbL 1.0. http:\/\/www.openstreetmap.org\/copyright","osm_type":"way","osm_id":"196097064","boundingbox":["45.5342072","45.5471073","6.2125133","6.2166785"],"lat":"45.5406069","lon":"6.2137878","display_name":"Route de l'Arclusaz, Chamoux-sur-Gelon, Chambéry, Savoie, Rhône-Alpes, France métropolitaine, 73390, France","class":"highway","type":"unclassified","importance":0.8}]

*/
// ===============


function geocoder_mapquest_nominatim_field($field, $field_item) {
  if ($field['type'] == 'text' || $field['type'] == 'text_long' || $field['type'] == 'text_with_summary' || $field['type'] == 'computed') {
    return geocoder_mapquest_nominatim($field_item['value']);
  }
  if ($field['type'] == 'addressfield' && module_exists('addressfield') && !addressfield_field_is_empty($field_item, $field)) {
    $address = geocoder_widget_parse_addressfield($field_item);
    return geocoder_mapquest_nominatim($address);
  }
  if ($field['type'] == 'location') {
    $address = geocoder_widget_parse_locationfield($field_item);
    return geocoder_mapquest_nominatim($address);
  }
  if ($field['type'] == 'taxonomy_term_reference') {
    $term = taxonomy_term_load($field_item['tid']);
    return geocoder_mapquest_nominatim($term->name);
  }
}

function _geocoder_mapquest_nominatim_geometry(&$data) {
  if (!isset($data[0]->lon)) {
    return NULL;
  }
  geophp_load();
  return new Point($data[0]->lon, $data[0]->lat);
}
