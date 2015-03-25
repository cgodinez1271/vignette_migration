<?php
/**
 * @file
 * Migrations for the Gallery content type.
*/

class PackageHubMigration extends XMLMigration {
	public function __construct($arguments) {
    	parent::__construct($arguments);
		$this->description = t('InStyle Gallery Content Type Migration');
		$this->dependencies = array('Tout', 'Gallery');
		
		$fields = array(
			'field_legacy_id' => t('Vignette component_id'), 
			'column_id' => t('Vignette column id'),
			'field_title' => t('Vignette title'),
			'field_headline' => t('Vignette ...'),
			'field_description' => t('Vignette description'),
			'field_carousel_tout_size' => t('Vignette ...'),
			'field_native' => t('Vignette ...'),
			'field_hub_type' => t('Vignette ...'),
			'field_creation_date' => t('Drupal Created timestamp'),
			'field_modified_date' => t('Drupal Changed timestamp'),
			'field_migrated' => t('Vignette ...'), 
			'field_hub_item' => t('Vignette ...'),
			'hub_items' => t('hubs grouping; populated by PrepareRow'), 
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
		$xml_folder = DRUPAL_ROOT . '/sites/default/files/migration/vignette/packagehub';
		$files = file_scan_directory($xml_folder, '/\.xml$/');
		$items_url = array_keys($files);
		// drush_print_r($items_url);

		$item_xpath = '/root/package-hub'; // relative to document
		$item_ID_xpath = 'fields/field_legacy_id'; // relative to item_xpath

        $source_options = array(
			'cache_counts' => TRUE,
		); 

		// Source and destination
		$this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
		
		$this->destination = new MigrateDestinationNode('packagehub');

		// Mapped fields
		$this->addFieldMapping('field_legacy_id', 'field_legacy_id')
			->xpath('fields/field_legacy_id');

		$this->addFieldMapping('title', 'field_title')
			->xpath('fields/field_title');

		$this->addFieldMapping('field_headline', 'field_headline')
			->xpath('fields/field_headline');

		$this->addFieldMapping('field_description', 'field_description')
			->xpath('fields/field_description');

		$this->addFieldMapping('field_carousel_tout_size', 'field_carousel_tout_size')
			->xpath('fields/field_carousel_tout_size');

		$this->addFieldMapping('field_hub_type', 'field_hub_type')
			->xpath('fields/field_hub_type');

		$this->addFieldMapping('created', 'field_creation_date')
			->xpath('fields/field_creation_date');

		$this->addFieldMapping('changed', 'field_modified_date')
			->xpath('fields/field_modified_date');

		$this->addFieldMapping('field_hub_items', 'hub_items')
			->xpath('hub_items')
		    ->separator(',');

		// Unmapped source fields
		$this->addFieldMapping('column_id')
			->issueGroup(t('DNM'));	
		$this->addFieldMapping('field_migrated')
			->issueGroup(t('DNM'));	

		// Unmapped destination fields

	}

	public function prepareRow($row) {
		// Always include this fragment at the beginning of every prepareRow()
		// implementation, so parent classes can ignore rows.
		if (parent::prepareRow($row) === FALSE) {
			return FALSE;
		}

		// return if there hubs components to prepare
		if (!isset($row->xml->components->field_hub_items->field_hub_item)) {
			return TRUE;
		}

		$hub_items_nids = array();

		foreach ($row->xml->components->field_hub_items->field_hub_item as $field_hub_item ) {
			$hub_legacy_id = (int) $field_hub_item;
			$hub_type = (string) $field_hub_item->attributes()->component_type;

			//drush_print_r($hub_legacy_id);
			//drush_print_r($hub_type);

			if ($hub_type == "gallery") {
				$hub_items_nids[] = (int) _isGalleryLegacyIDtoNID($hub_legacy_id);
			} elseif ($hub_type == "tout") {
				$hub_items_nids[] = (int) _isToutLegacyIDtoNID($hub_legacy_id);
			} else {
				watchdog('vignette_migrate','packagehub with field_legacy_id: %legacy_id has invalid hub type: %hub_type', array(
					'%legacy_id' => $row->field_legacy_id,
					'%hub_type' => $hub_type,
				), WATCHDOG_WARNING);
			}

		}

		//drush_print_r($hub_items_nids);
		$row->xml->hub_items = implode(",", $hub_items_nids);
		//drush_print_r($row);
		return TRUE;
	}
    
}
