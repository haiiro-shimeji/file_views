<?php

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
	
	abstract protected function add_field($table, $field, $alias = '', $params = array());
	
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
	
}

class views_plugin_file_views_query extends views_plugin_external_query {
	
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

	protected function add_field($table, $field, $alias = '', $params = array()) {
		return 'file_views_'.$field;
	}
	
	protected function compareObject( $a, $b ) {
		return ( $a->file_views_name == $b->file_views_name 
				? 0
				: ( $a->file_views_name < $b->file_views_name 
					? -1 : 1 ) 
		);
	}
	
}