<?php

class views_plugin_file_views_query extends views_plugin_query {

	/**
	 * A pager plugin that should be provided by the display.
	 */
	var $pager = NULL;

	//function init($base_table, $base_field, $options) {}	//@see view_plugin_query()

	function query($get_count = FALSE) {}

	function alter(&$view) {}

	function build(&$view) {
		
		$this->view = $view;

		$view->init_pager();

		$this->pager->query();

	}

	function execute(&$view) {
		
		$result = $this->getResults();

		$this->pager->total_items = count($result);
		if (!empty($this->pager->options['offset'])) {
			$this->pager->total_items -= $this->offset;
		}

		$this->pager->update_page_info();

		// Let the pager modify the query to add limits.
		$this->pager->pre_execute($this);

		usort( $result,
			function( $a, $b ) {
				return ( $a->file_views_name == $b->file_views_name 
						? 0
						: ( $a->file_views_name < $b->file_views_name 
							? -1 : 1 ) 
				);
			} 
		);

		$view->result = array_slice($result, $this->offset, ( 0 == $this->limit ? null : $this->limit ) );

		$this->pager->post_execute($view->result);

		if ($this->pager->use_pager()) {
		$view->total_rows = $this->pager->get_total_items();
		}
		
	}
	
	protected function getResults() {
		
		if ( ( $root_dir = preg_replace( '%/$%', '', variable_get('file_views_path', '') ) ) ) {
		
			$result = array();

			$dir = preg_replace( '%\.\.%', '', preg_replace( '%/$%', '', implode('/', $view->args) ) );
			$real_path = $root_dir.'/'.$dir;

			if (( $handle = opendir($real_path))) {

				while (false !== ($entry = readdir($handle))) {

					if ( preg_match( '%^\.%', $entry ) ) continue;
					
					$entry_real_path = $real_path.'/'.$entry;

					$result[] = (Object)array(
						'file_views_name' => $entry,
						'file_views_type' => ( is_dir( $entry_real_path ) ? 'd' : 'f' ),
						'file_views_path' => $dir.'/'.$entry.( is_dir( $entry_real_path ) ? '/' : ''),
						'file_views_modified' => 1000,
					);

				}

			}
			
			return $result;
			
		} else 
			
			return array();

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
