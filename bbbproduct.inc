<?php
/**
 * @file
 * Migrations for the BBBproduct content type.
*/

class bbbProductMigration extends XMLMigration {
	public function __construct($arguments) {
    	parent::__construct($arguments);
		$this->description = t('InStyle BBBProduct Content Type Migration');
		
		$fields = array(
			'field_legacy_id' => t('Vignette component_id'), 
			'column_id' => t('Not used in DCMS'),
			'field_title' => t('Drupal Title'),
			'field_headline' => t('Vignette headline'),
			'field_image' => t('Vignette image location'), 
			'field_image_width' => t('Not used in DCMS'),
			'field_image_height' => t('Not used in DCMS'),
			'field_description' => t('Vignette description'),
			'field_date' => t('Vignette creation date'),
			'field_alt_text' => t('Vignette alt_text'),
			'field_credit' => t('Vignette credit'),
			'field_migrated' => t('Not used in DCMS'),
			'field_brand' => ('Term'),
			'field_celebrity' => ('Term'),
			'field_year' => ('Term'),
			'field_trend' => ('Term'),
			'field_tag' => ('Term'),
			'field_product_type' => ('Term'),
			'field_buy_link' => ('Text'),
			'destination_dir' => 'Destination Directory; populated by PrepareRow',
		);

		$this->map = new MigrateSQLMap($this->machineName,
			array(
				'field_legacy_id' => array(
				'type' => 'int',
				'not null' => TRUE,
				'description' => 'Legacy ID',
				),
			),	
			MigrateDestinationNode::getKeySchema()
		);

		// XML File
		$xml_folder = DRUPAL_ROOT . '/sites/default/files/migration/vignette/bbbproduct';
		$files = file_scan_directory($xml_folder, '/\.xml$/');
		$items_url = array_keys($files);
		// drush_print_r($items_url);

		$item_xpath = '/root/bbbProduct'; // relative to document
		$item_ID_xpath = 'fields/field_legacy_id'; // relative to item_xpath

		$source_options = array(
			'cache_counts' => TRUE,
		); 

		// Source and destination
		$this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
		
		$this->destination = new MigrateDestinationNode('image');

		// Mapped fields
		$this->addFieldMapping('field_legacy_id', 'field_legacy_id')
			->xpath('fields/image_legacy_id');

		$this->addFieldMapping('field_image', 'field_image')
			->xpath('fields/field_image');
		$this->addFieldMapping('field_image:destination_dir', 'destination_dir');

		$this->addFieldMapping('title', 'field_title')
			->xpath('fields/field_title');
		
		$this->addFieldMapping('field_headline', 'field_headline')
			->xpath('fields/field_headline');
	
		$this->addFieldMapping('field_description', 'field_description')
			->xpath('fields/field_description');

		$this->addFieldMapping('field_edit_date', 'field_date')
			->xpath('fields/field_date');

		$this->addFieldMapping('field_alt_text', 'field_alt_text')
			->xpath('fields/field_alt_text');

		$this->addFieldMapping('field_credit', 'field_credit')
			->xpath('fields/field_credit');

		$this->addFieldMapping('field_buy_link', 'field_buy_link')
			->xpath('fields/field_buy_link');

		$this->addFieldMapping('field_tags:create_term')->defaultValue(TRUE);
    	$this->addFieldMapping('field_tags:source_type')->defaultValue('name');
		$this->addFieldMapping('field_tags:ignore_case')->defaultValue(FALSE);
		$this->addFieldMapping('field_tags', 'field_tag')
			->xpath('field_tags/field_tag');
//		$this->addFieldMapping('field_tags', 'field_trend')
//			->xpath('field_tags/field_trend');

    	$this->addFieldMapping('field_celebrity:source_type')->defaultValue('name');
		$this->addFieldMapping('field_celebrity:create_term')->defaultValue(TRUE);
		$this->addFieldMapping('field_celebrity:ignore_case')->defaultValue(FALSE);
		$this->addFieldMapping('field_celebrity', 'field_celebrity')
			->xpath('field_tags/field_celebrity');

    	$this->addFieldMapping('field_brand:source_type')->defaultValue('name');
		$this->addFieldMapping('field_brand:create_term')->defaultValue(TRUE);
		$this->addFieldMapping('field_brand:ignore_case')->defaultValue(FALSE);
		$this->addFieldMapping('field_brand', 'field_brand')
			->xpath('field_tags/field_brand');

    	$this->addFieldMapping('field_years:source_type')->defaultValue('name');
		$this->addFieldMapping('field_years:create_term')->defaultValue(TRUE);
    	$this->addFieldMapping('field_years:ignore')->defaultValue(FALSE);
		$this->addFieldMapping('field_years', 'field_year')
			->xpath('field_tags/field_year');

    	$this->addFieldMapping('field_product_type:source_type')->defaultValue('name');
		$this->addFieldMapping('field_product_type:create_term')->defaultValue(TRUE);
    	$this->addFieldMapping('field_product_type:ignore')->defaultValue(FALSE);
		$this->addFieldMapping('field_product_type', 'field_product_type')
			->xpath('field_tags/field_product_type');

		// Unmapped source fields
        $this->addFieldMapping('is_published')
		     ->issueGroup(t('DNM'));
        $this->addFieldMapping('field_image_width')
		     ->issueGroup(t('DNM'));
        $this->addFieldMapping('field_image_height')
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

		$image_path = (string) $row->xml->fields->field_image;
		$file = basename($image_path);
		$file_info = pathinfo($file);

		// No file extension? No import.
		if (!isset($file_info['extension']) || !$file_info['extension']) {
			return FALSE;
		}

		$valid_image_extensions = array(
			'png',
			'gif',
			'jpg',
			'jpeg',
		);

		if (!in_array($file_info['extension'], $valid_image_extensions)) {
			return FALSE;
		}

		$image_exploded = explode('/', $image_path);

		$file_name = array_pop($image_exploded);
		$row->title = $file_name;

		$subdir = $image_exploded[4] . '/' . $image_exploded[5] . '/' . $image_exploded[6];	
		$row->destination_dir = "public://$subdir";
	}
}
