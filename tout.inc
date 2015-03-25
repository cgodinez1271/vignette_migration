<?php
/**
 * @file
 * Migrations for the Tout content type.
*/

class ToutMigration extends XMLMigration {
	public function __construct($arguments) {
    	parent::__construct($arguments);
		$this->description = t('InStyle Tout Content Type Migration');
		$this->dependencies = array('ImageComponent');
				
		$fields = array(
			'field_legacy_id' => t('Vignette component_id'), 
			'column_id' => t('Not in DCMS'), 
			'field_title' => t('Vignette headline'),
			'field_headline' => t('Vignette headline'),
			'field_category' => t('Vignette Channel'),
			'field_subcategory' => t('Vignette SubChannel'), 
			'field_images' => t('Vignette image'), 
			'field_external_link' => t('Vignette image'), 
			'field_creation_date' => t('Drupal Created timestamp'),
			'field_modified_date' => t('Drupal Changed timestamp'),
			'field_link_text' => t('Not used in DCMS '), 
			'field_native' => t('Vignette image'), 
			'field_tout_type' => t('Vignette image'), 
			'field_migrated' => t('Vignette image'), 
		);

		$this->map = new MigrateSQLMap($this->machineName,
			array(
				'field_legacy_id' => array(
				'type' => 'int',
				'not null' => TRUE,
				),
			),	
			MigrateDestinationNode::getKeySchema()
		);

		// XML File
		$xml_folder = DRUPAL_ROOT . '/sites/default/files/migration/vignette/tout';
		$files = file_scan_directory($xml_folder, '/\.xml$/');
		$items_url = array_keys($files);
		// drush_print_r($items_url);

		$item_xpath = '/root/tout';  // relative to document
		$item_ID_xpath = 'fields/field_legacy_id'; // relative to item_xpath

        $source_options = array(
			'cache_counts' => TRUE,
		); 

		// Source and destination
		$this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
		
		$this->destination = new MigrateDestinationNode('tout');

		// Mapped fields
		$this->addFieldMapping('field_legacy_id', 'field_legacy_id')
			->xpath('fields/field_legacy_id');

		$this->addFieldMapping('title', 'field_title')
			->xpath('fields/field_title');

		$this->addFieldMapping('field_headline', 'field_headline')
			->xpath('fields/field_headline');

		$this->addFieldMapping('field_category', 'field_category')
			->xpath('fields/field_category')
			->arguments(array('source_type' => 'name'));

		$this->addFieldMapping('field_subcategory', 'field_subcategory')
			->xpath('fields/field_subcategory')
			->arguments(array('source_type' => 'name'));

		$this->addFieldMapping('field_images', 'field_images')
			->xpath('fields/field_images')
		    ->sourceMigration('ImageComponent')
		    ->arguments(array('source_type' => 'nid'));

		$this->addFieldMapping('field_external_link', 'field_external_link')
			->xpath('fields/field_external_link');

		$this->addFieldMapping('field_native', 'field_native')
			->xpath('fields/field_native');

		$this->addFieldMapping('created', 'field_creation_date')
			->xpath('fields/field_creation_date');

		$this->addFieldMapping('changed', 'field_modified_date')
			->xpath('fields/field_modified_date');

		$this->addFieldMapping('field_tout_type', 'field_tout_type')
			->xpath('fields/field_tout_type');


		// Unmapped source fields
        $this->addFieldMapping('column_id')                                   
			->issueGroup(t('DNM'));	
        $this->addFieldMapping('field_migrated')
			->issueGroup(t('DNM'));	
        $this->addFieldMapping('field_link_text')
			->issueGroup(t('DNM'));	

		// TODO: Unmapped destination fields

	}

	public function prepareRow($row) {
		// Always include this fragment at the beginning of every prepareRow()
		// implementation, so parent classes can ignore rows.
		if (parent::prepareRow($row) === FALSE) {
			return FALSE;
		}   

		// translate true/false to a boolean	
		$fn = $row->xml->fields->field_native;
		$row->xml->fields->field_native = filter_var($fn, FILTER_VALIDATE_BOOLEAN);
		//drush_print_r($row);
		return TRUE;
	}
}
