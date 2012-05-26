<?php

class views_plugin_file_views_query extends views_plugin_query {

	/**
	 * A pager plugin that should be provided by the display.
	 */
	var $pager = NULL;

	//function init($base_table, $base_field, $options) {}	//@see view_plugin_query()

	function query($get_count = FALSE) {}

	function alter(&$view) {}

	function build(&$view) {}

	function execute(&$view) {
		
		$view->result[] = (Object)array(
			'file_views_name' => 'hoge',
			'file_views_path' => 'hoge/fuga',
			'file_views_modified' => 1000,
		);
		
	}

	function add_field($table, $field, $alias = '', $params = array()) {
		return 'file_views_'.$field;
	}

	function add_signature(&$view) {}

	function get_aggregation_info() {}

	function options_form(&$form, &$form_state) {
		parent::options_form($form, $form_state);
	}

	function options_validate(&$form, &$form_state) {}

	function options_submit(&$form, &$form_state) {}

	function summary_title() {
		return t('Settings');
	}

	function set_limit($limit) {
		$this->limit = $limit;
	}

	function set_offset($offset) {
		$this->offset = $offset;
	}

	function render_pager($exposed_input) {
		if (!empty($this->pager) && $this->pager->use_pager()) {
			return $this->pager->render($exposed_input);
		}

		return '';
	}

	function set_where_group($type = 'AND', $group = NULL, $where = 'where') {
		// Set an alias.
		$groups = &$this->$where;

		if (!isset($group)) {
			$group = empty($groups) ? 1 : max(array_keys($groups)) + 1;
		}

		// Create an empty group
		if (empty($groups[$group])) {
			$groups[$group] = array('conditions' => array(), 'args' => array());
		}

		$groups[$group]['type'] = strtoupper($type);
		return $group;
	}

	function set_group_operator($type = 'AND') {
		$this->group_operator = strtoupper($type);
	}

	function get_result_entities($results, $relationship = NULL) {
		return FALSE;
	}

}
