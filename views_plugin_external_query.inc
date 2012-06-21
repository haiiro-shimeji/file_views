<?php

if ( !class_exists( "views_plugin_external_query" ) ) {
	
	/**
	* Views query plugin that take the extenal data instead of fetch from database.
	* 
	*/
	abstract class views_plugin_external_query extends views_plugin_query {

		/**
		* A pager plugin that should be provided by the display.
		*/
		var $pager = NULL;

		/**
		* Returns datas.
		* 
		* @return array of object result data. 
		*/
		abstract protected function getResults();

		abstract protected function _add_field($table, $field, $alias = '', $params = array());

		/**
		* Default compare function used by usort(). 
		*/
		abstract protected function compareObject( $a, $b );

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

			usort( $result, array( $this, "compareObject" ) );

			$view->result = array_slice($result, $this->offset, ( 0 == $this->limit ? null : $this->limit ) );

			$this->pager->post_execute($view->result);

			if ($this->pager->use_pager()) {
			$view->total_rows = $this->pager->get_total_items();
			}

		}

		function add_field($table, $field, $alias = '', $params = array()) {
			return $this->_add_field($table, $field, $alias, $params);
		}

	}
	
}
