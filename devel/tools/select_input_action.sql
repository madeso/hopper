
/*
select confnum, input_action_conf.action_id, action_name, configurable
 from  input_action_conf natural join input_action;
*/

select confnum, action_id, action_name, configurable, action_type, method, periph_id, periph_value, periph_value2, reversed, action_parameter from input_action natural join input_action_conf where confnum=0;
  
