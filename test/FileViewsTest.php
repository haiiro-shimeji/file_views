<?php

define( 'FILEVIEWS_TEST', true );
require_once dirname(__FILE__).'/../classes/Entity.php';

class FileViewTest extends PHPUnit_Framework_TestCase {

	public function testNewEntities() {
		
		FileViews\MockFind::$closure = function() {
			return array( "1000 hoge", "2000 fuga" );
		};
		
		$entities = FileViews\Entity::newEntities( strtotime("yesterday") );
		
		$this->assertTrue( 2 == count( $entities ) );
		
		$this->assertTrue( "hoge" == $entities[0]->getPath() );
		$this->assertTrue( "fuga" == $entities[1]->getPath() );
		
	}
	
	public function testRegister() {
		
		$entity = new FileViews\Entity;
		$entity->name = "hoge";
		$entity->path = "path";
		$entity->modified = 5;
		
		$entity->register();
		
		$entity2 = new FileViews\Entity( $entity->getId() );
		$this->assertTrue( "hoge" == $entity2->getName() );
		$this->assertTrue( "path" == $entity2->getPath() );
		$this->assertTrue( 5 == $entity2->getModified() );
		
	}

	public function testUpdate() {
		
		$entity = new FileViews\Entity;
		$entity->name = "hoge";
		$entity->path = "path";
		$entity->modified = 5;
		
		$entity->register();
		
		$entity->name = "hogehoge";
		$entity->path = "fugafuga";
		$entity->modified = 8;
		
		$entity->register();
		
		$entity2 = new FileViews\Entity( $entity->getId() );
		$this->assertTrue( "hogehoge" == $entity2->getName() );
		$this->assertTrue( "fugafuga" == $entity2->getPath() );
		$this->assertTrue( 8 == $entity2->getModified() );
		
	}

	public function testDelete() {
		
		$entity = new FileViews\Entity;
		$entity->name = "hoge";
		$entity->path = "path";
		$entity->modified = 5;
		
		$entity->register();
		
		new FileViews\Entity( $entity->getId() );
		
		$entity->delete();
		
		try {
			new FileViews\Entity( $entity->getId() );
			$this->assertTrue( false );
		} catch ( Exception $e ) {}
		
	}

}
